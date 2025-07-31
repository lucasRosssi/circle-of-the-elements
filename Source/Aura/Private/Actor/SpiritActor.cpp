// Copyright Lucas Rossi


#include "Actor/SpiritActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Components/SphereComponent.h"
#include "Interfaces/AbilityInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

ASpiritActor::ASpiritActor()
{
  PrimaryActorTick.bCanEverTick = true;

  SpiritCollision = CreateDefaultSubobject<USphereComponent>("SpiritCollision");
  SpiritCollision->SetupAttachment(GetRootComponent());
  SpiritCollision->SetSphereRadius(10.f, false);
  SpiritCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
  SpiritCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  SpiritCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpiritActor::SetAbilityTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Abilities))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Ability tag: %s"), *InTag.ToString())
  }
  
  AbilityTag = InTag;
}

void ASpiritActor::SetElementTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Abilities_Element))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Element tag: %s"), *InTag.ToString())
  }
  
  ElementTag = InTag;
}

void ASpiritActor::SetCooldownTag(const FGameplayTag& InTag)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Cooldown))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Cooldown tag: %s"), *InTag.ToString())
  }
  
  CooldownTag = InTag;
}

void ASpiritActor::SetChargeTagAndCount(const FGameplayTag& InTag, int32 ChargesCount)
{
  if (!InTag.MatchesTag(FAuraGameplayTags::Get().Charges))
  {
    UE_LOG(LogAura, Error, TEXT("[SpiritActor] Invalid Charge tag: %s"), *InTag.ToString())
  }
  
  ChargeTag = InTag;
  MaxChargesCount = ChargesCount;
}

void ASpiritActor::SetHijacker(AActor* InHijacker)
{
  if (IsValid(Hijacker))
  {
    SpiritCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ICombatInterface* CombatHijacker = Cast<ICombatInterface>(Hijacker);
    CombatHijacker->GetOnDeathDelegate().RemoveDynamic(this, &ASpiritActor::OnHijackerDeath);
  }
  
  Hijacker = InHijacker;
  OnHijackerSetDelegate.Broadcast(Hijacker);

  if (Hijacker == nullptr && !GetSpiritNiagara()->IsActive())
  {
    GetSpiritNiagara()->Activate();
  }
  
  if (!IsValid(Hijacker) || !Hijacker->Implements<UCombatInterface>()) return;

  SpiritCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  
  ICombatInterface* CombatHijacker = Cast<ICombatInterface>(Hijacker);
  CombatHijacker->GetOnDeathDelegate().AddUniqueDynamic(this, &ASpiritActor::OnHijackerDeath);
}

UNiagaraComponent* ASpiritActor::GetSpiritNiagara_Implementation()
{
  return nullptr;
}

FVector ASpiritActor::GetAbilityUseLocation()
{
  if (!GetOwnerASC() || !Ability) return FVector();

  const FName SocketName = IAbilityInterface::Execute_GetActiveAbilitySocketName(Ability);
  const bool bSocketInWeapon = IAbilityInterface::Execute_IsAbilityUsingWeapon(Ability);

  return ICombatInterface::Execute_GetAbilitySocketLocation(GetOwner(), SocketName, bSocketInWeapon);
}

FVector ASpiritActor::GetHijackLocation()
{
  return ICombatInterface::Execute_GetAbilitySocketLocation(Hijacker, FName("SpiritHijackSocket"), true);
}

void ASpiritActor::BeginPlay()
{
  Super::BeginPlay();

  ListenForCooldownChange();
  ListenForAbilityActivation();
  SpiritCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpiritActor::OnHijackerOverlap);
}

void ASpiritActor::ListenForCooldownChange()
{
  if (!GetOwnerASC()) return;

  if (ChargeTag.IsValid())
  {
    OwnerASC->RegisterGameplayTagEvent(
      ChargeTag,
      EGameplayTagEventType::AnyCountChange
    ).AddUObject(this, &ASpiritActor::ChargeTagChanged);
  }
  else if (CooldownTag.IsValid())
  {
    OwnerASC->RegisterGameplayTagEvent(
      CooldownTag,
      EGameplayTagEventType::NewOrRemoved
    ).AddUObject(this, &ASpiritActor::CooldownTagChanged);
  }
}

void ASpiritActor::ListenForAbilityActivation()
{
  if (!GetOwnerASC() || !AbilityTag.IsValid()) return;

  OnAnyAbilityActivated = OwnerASC->AbilityActivatedCallbacks.AddUObject(this, &ASpiritActor::AbilityActivated);
}

void ASpiritActor::OnHijackerDeath(AActor* DeadHijacker)
{
  SetHijacker(nullptr);
}

void ASpiritActor::OnHijackerOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex,
  bool bFromSweep,
  const FHitResult& SweepResult
)
{
  if (Hijacker && Hijacker != OtherActor) return;

  UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Hijacker);

  if (!ASC) return;

  FGameplayEventData Payload = FGameplayEventData();

  ASC->HandleGameplayEvent(
    FAuraGameplayTags::Get().Event_Spirit_Hijacked,
    &Payload
  );

  GetSpiritNiagara()->Deactivate();
}

void ASpiritActor::AbilityActivated(UGameplayAbility* InAbility)
{
  if (InAbility->GetAssetTags().HasTagExact(AbilityTag))
  {
    bUsingAbility = true;
    Ability = Cast<UBaseAbility>(InAbility);
    if (Ability)
    {
      Ability->OnAbilityStateChanged.AddUObject(this, &ASpiritActor::AbilityStateChanged);
      OwnerASC->AbilityActivatedCallbacks.Remove(OnAnyAbilityActivated);
    }
  }
}

void ASpiritActor::AbilityStateChanged(bool bActivated)
{
  bUsingAbility = bActivated;
}

void ASpiritActor::CooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
  if (NewCount > 0)
  {
    GetSpiritNiagara()->Deactivate();
    SetActorTickEnabled(false);
  }
  else
  {
    GetSpiritNiagara()->Activate();
    SetActorTickEnabled(true);
  }
}

void ASpiritActor::ChargeTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
  if (!GetOwnerASC()) return;

  const FGameplayEffectQuery Query = FGameplayEffectQuery
    ::MakeQuery_MatchAnyOwningTags(ChargeTag.GetSingleTagContainer());

  const TArray<FActiveGameplayEffectHandle> Handles = OwnerASC->GetActiveEffects(Query);

  const int32 CurrentCount = Handles.IsEmpty() ? 0 : OwnerASC->GetCurrentStackCount(Handles[0]);

  if (CurrentCount == 0)
  {
    GetSpiritNiagara()->Deactivate();
    SetActorTickEnabled(false);
  }
  else
  {
    const float NewScale = (CurrentCount + 0.f) / (MaxChargesCount + 0.f);
    for (const FName& ParameterName : NiagaraScaleParameters)
    {
      GetSpiritNiagara()->SetFloatParameter(ParameterName, NewScale);
    }
    
    if (!GetSpiritNiagara()->IsActive())
    {
      GetSpiritNiagara()->Activate();
      SetActorTickEnabled(true);
    }
  }

}

UAbilitySystemComponent* ASpiritActor::GetOwnerASC()
{
  if (GetOwner() == nullptr) return nullptr;

  if (OwnerASC == nullptr)
  {
    OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
  }

  return OwnerASC;
}
