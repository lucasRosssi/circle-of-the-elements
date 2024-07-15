// Copyright Lucas Rossi


#include "Actor/AreaEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Game/TeamComponent.h"

// Sets default values
AAreaEffectActor::AAreaEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	TeamComponent = CreateDefaultSubobject<UTeamComponent>("TeamComponent");
	TeamComponent->TeamID = NEUTRAL_TEAM;
}

void AAreaEffectActor::BeginPlay()
{
	Super::BeginPlay();

	if (LifeSpan > 0.f)
	{
		SetLifeSpan(LifeSpan);
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
}

void AAreaEffectActor::BeginDestroy()
{
	for (auto HandlePair = ActiveEffectHandles.CreateIterator(); HandlePair; ++HandlePair)
	{
		HandlePair->Value->RemoveActiveGameplayEffect(HandlePair->Key, -1);
		HandlePair.RemoveCurrent();
	}

	if (PeriodicEffectTimer.IsValid()) PeriodicEffectTimer.Invalidate();

	if (UNiagaraComponent* NiagaraComponent = GetComponentByClass<UNiagaraComponent>())
	{
		NiagaraComponent->Deactivate();
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(
			DestroyTimer,
			[this]()
			{
				Super::BeginDestroy();
			},
			2.f,
			false,
			2.f
			);
	}
	else
	{
		Super::BeginDestroy();
	}
}

void AAreaEffectActor::ApplyEffectToTarget(
	UAbilitySystemComponent* TargetASC
	)
{
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec
	(GameplayEffectClass, 
	ActorLevel, 
	EffectContextHandle);
	
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf
	(*EffectSpecHandle
	.Data.Get());

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
		Destroy();
	}
}

void AAreaEffectActor::OnOverlap(AActor* TargetActor)
{
	if (
		TargetTeam == ETargetTeam::Enemies &&
		!UAuraAbilitySystemLibrary::AreActorsEnemies(this, TargetActor)
		) return;
	if (
		TargetTeam == ETargetTeam::Friends &&
		!UAuraAbilitySystemLibrary::AreActorsFriends(this, TargetActor)
		) return;
	UAbilitySystemComponent* TargetASC = 
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return;
	
	ASCsInArea.Add(TargetASC);
	
	if (AbilityParams.IsValid())
	{
		bool bSuccess;
		ApplyAbilityEffect(TargetASC, bSuccess);
	}

	if (GameplayEffectClass != nullptr)
	{
		ApplyEffectToTarget(TargetASC);
	}
}

void AAreaEffectActor::ApplyAbilityEffect(UAbilitySystemComponent* TargetASC, bool& bSuccess)
{
		AbilityParams.TargetASC = TargetASC;
		UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
}

void AAreaEffectActor::ApplyAbilityEffectToActorsInArea()
{
	bool bSuccess;
	for (const auto ASC : ASCsInArea)
	{
		if (!IsValid(ASC)) continue;

		ApplyAbilityEffect(ASC, bSuccess);
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
