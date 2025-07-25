// Copyright Lucas Rossi


#include "AbilitySystem/Components/StatusEffectsManager.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Data/StatusEffectInfo.h"
#include "Enums/StatusEffectPosition.h"
#include "Interfaces/CombatInterface.h"
#include "Utils/AuraSystemsLibrary.h"

class ICombatInterface;

UStatusEffectsManager::UStatusEffectsManager()
{
	UActorComponent::SetComponentTickEnabled(false);
}

void UStatusEffectsManager::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary
		::GetAbilitySystemComponent(GetOwner());

	if (ASC)
	{
		RegisterStatusEffectTagEvent(ASC);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddUObject(
			this,
			&UStatusEffectsManager::RegisterStatusEffectTagEvent
		);
	}
}

void UStatusEffectsManager::RegisterStatusEffectTagEvent(UAbilitySystemComponent* InASC)
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	const TArray<FGameplayTag> StatusEffectTags = *Tags.ParentsToChildren.Find(Tags.StatusEffects);
	for (const auto Tag : StatusEffectTags)
	{
		InASC->RegisterGameplayTagEvent(
			Tag,
			EGameplayTagEventType::NewOrRemoved
			).AddUObject(this, &UStatusEffectsManager::StatusEffectTagChanged);
	}
}

void UStatusEffectsManager::StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnActivateStatusEffect(CallbackTag);
	}
	else
	{
		OnDeactivateStatusEffect(CallbackTag);
	}
}

void UStatusEffectsManager::OnActivateStatusEffect(const FGameplayTag& StatusEffectTag)
{

	if (!GetOwner()->HasAuthority() || ICombatInterface::Execute_IsDead(GetOwner())) return;
		
	const UStatusEffectInfo* StatusEffectInfo = UAuraSystemsLibrary
		::GetStatusEffectInfo(GetOwner());
	if (!StatusEffectInfo->StatusEffects.Contains(StatusEffectTag)) return;
	
	const FStatusEffectData& Data = *StatusEffectInfo->StatusEffects.Find(StatusEffectTag);
	if (IsValid(Data.NiagaraSystem))
	{
		MulticastActivateStatusEffect(
			StatusEffectTag,
			Data
			);
	}
}

void UStatusEffectsManager::OnDeactivateStatusEffect(const FGameplayTag& StatusEffectTag)
{
	if (!ActiveNiagaraComponents.Contains(StatusEffectTag)) return;

	UNiagaraComponent* NiagaraComponent = *ActiveNiagaraComponents.Find(StatusEffectTag);
	if (NiagaraComponent)
	{
		ActiveNiagaraComponents.Remove(StatusEffectTag);
		NiagaraComponent->Deactivate();
	}
}

void UStatusEffectsManager::MulticastActivateStatusEffect_Implementation(
	const FGameplayTag& StatusEffectTag,
	const FStatusEffectData& StatusData
	)
{
	USceneComponent* AttachmentComponent;
	switch (StatusData.Position)
	{
	case EStatusEffectPosition::Top:
		{
			AttachmentComponent = ICombatInterface
				::Execute_GetTopStatusEffectSceneComponent(GetOwner());
			break;
		}
	case EStatusEffectPosition::Bottom:
		{
			AttachmentComponent = ICombatInterface
				::Execute_GetBottomStatusEffectSceneComponent(GetOwner());
			break;
		}
	case EStatusEffectPosition::Center:
	default:
		{
			AttachmentComponent = GetOwner()->GetRootComponent();
		}
	}
	
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				StatusData.NiagaraSystem,
				AttachmentComponent,
				FName(),
				FVector(0),
				FRotator(0),
				EAttachLocation::KeepRelativeOffset,
				true
			);
	NiagaraComponent->AutoAttachRotationRule = EAttachmentRule::KeepWorld;

	ActiveNiagaraComponents.Add(StatusEffectTag, NiagaraComponent);
}
