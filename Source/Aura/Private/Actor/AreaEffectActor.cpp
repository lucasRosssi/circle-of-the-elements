// Copyright Lucas Rossi


#include "Actor/AreaEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "Components/TeamComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAreaEffectActor::AAreaEffectActor()
{
  PrimaryActorTick.bCanEverTick = true;
  bReplicates = true;

  SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
  TeamComponent = CreateDefaultSubobject<UTeamComponent>("TeamComponent");
  TeamComponent->TeamID = NEUTRAL_TEAM;
  RadialForceSphere = CreateDefaultSubobject<USphereComponent>("RadialForceSphere");
  RadialForceSphere->SetupAttachment(GetRootComponent());
}

void AAreaEffectActor::BeginPlay()
{
  Super::BeginPlay();

  if (SpawnSound)
  {
    UGameplayStatics::PlaySoundAtLocation(
      this,
      SpawnSound,
      GetActorLocation(),
      GetActorRotation(),
      SpawnSoundVolume,
      SpawnSoundPitch
    );
  }

  if (bHasRadialForce)
  {
    RadialForceSphere->OnComponentBeginOverlap.AddDynamic(this, &AAreaEffectActor::OnRadialForceOverlap);
    RadialForceSphere->OnComponentEndOverlap.AddDynamic(this, &AAreaEffectActor::OnRadialForceEndOverlap);
  }

  if (LifeSpan > 0.f)
  {
    SetLifeSpan(LifeSpan + DelayDestroy);
    if (UNiagaraComponent* NiagaraComponent = GetComponentByClass<UNiagaraComponent>())
    {
      FTimerHandle NiagaraDeactivateTimer;
      GetWorld()->GetTimerManager().SetTimer(
        NiagaraDeactivateTimer,
        [this, NiagaraComponent]()
        {
          bDestroying = true;
          NiagaraComponent->Deactivate();
          if (PeriodicEffectTimer.IsValid()) PeriodicEffectTimer.Invalidate();
        },
        LifeSpan,
        false,
        LifeSpan
      );
    }
  }

  if (!bInstant)
  {
    GetWorld()->GetTimerManager().SetTimer(
      PeriodicEffectTimer,
      this,
      &AAreaEffectActor::ApplyAbilityEffectToActorsInArea,
      Period,
      true
    );
  }

  if (DelayImpact > 0.f)
  {
    FTimerHandle ImpactTimer;
    GetWorld()->GetTimerManager().SetTimer(
      ImpactTimer,
      this,
      &AAreaEffectActor::ApplyAbilityEffectToActorsInArea,
      DelayImpact,
      false
    );
  }
}

void AAreaEffectActor::BeginDestroy()
{
  for (auto HandlePair = ActiveEffectHandles.CreateIterator(); HandlePair; ++HandlePair)
  {
    if (!IsValid(HandlePair.Value())) continue;
    HandlePair.Value()->RemoveActiveGameplayEffect(HandlePair->Key, -1);
    HandlePair.RemoveCurrent();
  }

  Super::BeginDestroy();
}

void AAreaEffectActor::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);

  if (bHasRadialForce)
  {
    for (const auto Actor : ActorsInForceArea)
    {
      ICombatInterface::Execute_ApplyAttraction(
        Actor,
        RadialForceSphere->GetComponentLocation(),
        DeltaSeconds,
        Force
        );
    }
  }
}

void AAreaEffectActor::DeactivateAndDestroy()
{
  if (UNiagaraComponent* NiagaraComponent = GetComponentByClass<UNiagaraComponent>())
  {
    bDestroying = true;
    NiagaraComponent->Deactivate();

    FTimerHandle DestroyTimer;
    GetWorld()->GetTimerManager().SetTimer(
      DestroyTimer,
      [this]()
      {
        Destroy();
      },
      DelayDestroy,
      false,
      DelayDestroy
    );
  }
  else
  {
    Destroy();
  }
}

void AAreaEffectActor::ApplyEffectToTarget(
  UAbilitySystemComponent* TargetASC
)
{
  FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
  EffectContextHandle.AddSourceObject(this);

  const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
    GameplayEffectClass,
    ActorLevel,
    EffectContextHandle
  );

  const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
    *EffectSpecHandle
     .Data.Get()
  );

  const UGameplayEffect* Def = EffectSpecHandle.Data.Get()->Def.Get();
  const bool bIsInfinite = Def->DurationPolicy == EGameplayEffectDurationType::Infinite;

  if (bIsInfinite)
  {
    ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
  }

  if (
    bInstant && !bIsInfinite
  )
  {
    DeactivateAndDestroy();
  }
}

void AAreaEffectActor::OnOverlap(AActor* TargetActor)
{
  if (bDestroying) return;
  if (
    TargetTeam == ETargetTeam::Enemies &&
    !UAuraAbilitySystemLibrary::AreActorsEnemies(this, TargetActor)
  )
    return;
  if (
    TargetTeam == ETargetTeam::Friends &&
    !UAuraAbilitySystemLibrary::AreActorsFriends(this, TargetActor)
  )
    return;
  UAbilitySystemComponent* TargetASC =
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
  if (!IsValid(TargetASC)) return;

  ASCsInArea.Add(TargetASC);

  // When the ability has a delayed impact it'll apply effects when impact happens
  if (AbilityParams.IsValid() && DelayImpact == 0.f)
  {
    bool bSuccess;
    ApplyAbilityEffect(TargetASC, bSuccess);
  }

  if (GameplayEffectClass != nullptr && DelayImpact == 0.f)
  {
    ApplyEffectToTarget(TargetASC);
  }
}

void AAreaEffectActor::ApplyAbilityEffect(UAbilitySystemComponent* TargetASC, bool& bSuccess)
{
  AbilityParams.TargetASC = TargetASC;
  AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
    AbilityParams.AreaOrigin,
    TargetASC->GetAvatarActor()->GetActorLocation()
  );
  UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
}

void AAreaEffectActor::ApplyAbilityEffectToActorsInArea()
{
  bool bSuccess;
  for (auto ASC = ASCsInArea.CreateIterator(); ASC; ++ASC)
  {
    UAbilitySystemComponent* CurrentASC = *ASC;
    if (!IsValid(CurrentASC))
    {
      ASC.RemoveCurrent();
    }

    ApplyAbilityEffect(CurrentASC, bSuccess);
  }
}

void AAreaEffectActor::OnEndOverlap(AActor* TargetActor)
{
  UAbilitySystemComponent* TargetASC =
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
  if (!IsValid(TargetASC)) return;

  if (EndOverlapGameplayEffectClass != nullptr)
  {
    ApplyEffectToTarget(TargetASC);
  }

  for (auto HandlePair = ActiveEffectHandles.CreateIterator(); HandlePair; ++HandlePair)
  {
    if (TargetASC == HandlePair->Value)
    {
      TargetASC->RemoveActiveGameplayEffect(HandlePair->Key, -1);
      HandlePair.RemoveCurrent();
    }
  }

  ASCsInArea.Remove(TargetASC);
}

void AAreaEffectActor::OnRadialForceOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
  if (bDestroying) return;
  if (
    TargetTeam == ETargetTeam::Enemies &&
    !UAuraAbilitySystemLibrary::AreActorsEnemies(this, OtherActor)
  )
    return;
  if (
    TargetTeam == ETargetTeam::Friends &&
    !UAuraAbilitySystemLibrary::AreActorsFriends(this, OtherActor)
  )
    return;
  const UAbilitySystemComponent* TargetASC =
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
  if (!IsValid(TargetASC)) return;

  ActorsInForceArea.Add(OtherActor);
}
void AAreaEffectActor::OnRadialForceEndOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex
    )
{
  ActorsInForceArea.Remove(OtherActor);
}
