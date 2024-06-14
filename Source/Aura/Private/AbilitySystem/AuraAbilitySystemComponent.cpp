// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/ActiveAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(
		this,
		&UAuraAbilitySystemComponent::EffectApplied
	);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& InAbilities
)
{
	for (const auto AbilityClass : InAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const auto* ActiveAbility = Cast<UActiveAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ActiveAbility->StartupInputTag);
			SetAbilityStatusFromSpec(AbilitySpec, FAuraGameplayTags::Get().Abilities_Status_Equipped);
		}
		GiveAbility(AbilitySpec);
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		AbilitySpecInputPressed(AbilitySpec);
		
		if (AbilitySpec.IsActive()) continue;
		
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const auto AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

AAuraCharacterBase* UAuraAbilitySystemComponent::GetAvatarCharacter()
{
	if (AvatarCharacter == nullptr)
	{
		AvatarCharacter = CastChecked<AAuraCharacterBase>(GetAvatarActor());
	}
	
	return AvatarCharacter;
}

void UAuraAbilitySystemComponent::SetExclusiveGameplayTagFromSpec(
	FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& GameplayTag,
	FName ParentTag
	)
{
	const FGameplayTag ParentStatusTag = FGameplayTag::RequestGameplayTag(ParentTag);
	if (!GameplayTag.MatchesTag(ParentStatusTag))
	{
		UE_LOG(LogAura, Error, TEXT("Trying to set an invalid gameplay tag for the provided parent tag"));
		return;	
	}
	
	if (AbilitySpec.DynamicAbilityTags.HasTag(ParentStatusTag))
	{
		TArray<FGameplayTag> Tags;
		AbilitySpec.DynamicAbilityTags.GetGameplayTagArray(Tags);
		for (auto Tag : Tags)
		{
			if (Tag.MatchesTag(ParentStatusTag))
			{
				AbilitySpec.DynamicAbilityTags.RemoveTag(Tag);
			}
		}
	}
	AbilitySpec.DynamicAbilityTags.AddTag(GameplayTag);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(
	const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (auto Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(
	const FGameplayAbilitySpec& AbilitySpec
)
{
	for (auto Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusFromSpec(
	const FGameplayAbilitySpec& AbilitySpec
)
{
	for (auto Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::SetAbilityStatusFromSpec(
	FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& StatusTag
)
{
	SetExclusiveGameplayTagFromSpec(
		AbilitySpec,
		StatusTag,
		FName("Abilities.Status")
		);
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(
	const FGameplayTag& AbilityTag
)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityStatusFromSpec(*Spec);
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(
	const FGameplayTag& AbilityTag
	)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}

	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(
	const FGameplayTag& AbilityTag
	)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability.Get()->AbilityTags.HasTagExact(AbilityTag))
		{
			return &AbilitySpec;
		}
	}

	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (
		!GetAvatarActor()->Implements<UPlayerInterface>() ||
		IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) <= 0
	) return;

	ServerUpgradeAttribute(AttributeTag);
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(
	const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActor(),
		AttributeTag,
		Payload
	);

	IPlayerInterface::Execute_SpendAttributePoints(GetAvatarActor(), 1);
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());

	for (
		const auto& Info :
		AbilityInfo->FindAbilitiesFromCharacter(GetAvatarCharacter()->GetCharacterName()))
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FAuraGameplayTags Tags = FAuraGameplayTags::Get();
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			SetAbilityStatusFromSpec(AbilitySpec, Tags.Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityState(Info.AbilityTag, Tags.Abilities_Status_Eligible);
		}
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& InputTag
	)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	if (!AbilitySpec) return;

	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	FGameplayTagContainer AllowedStatuses = FGameplayTagContainer();
	AllowedStatuses.AddTag(Tags.Abilities_Status_Equipped);
	AllowedStatuses.AddTag(Tags.Abilities_Status_Unlocked);
	
	const FGameplayTag& Status = GetAbilityStatusFromSpec(*AbilitySpec);
	if (!Status.MatchesAnyExact(AllowedStatuses)) return;
	
	const FGameplayTag& PreviousInputTag = GetInputTagFromSpec(*AbilitySpec);

	RemoveInputTagFromAbilities(InputTag);
	SetInputTagFromSpec(*AbilitySpec, InputTag);
	SetAbilityStatusFromSpec(*AbilitySpec, Tags.Abilities_Status_Equipped);

	MarkAbilitySpecDirty(*AbilitySpec);
	ClientEquipAbility(AbilityTag, Tags.Abilities_Status_Equipped, InputTag, PreviousInputTag);
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag,
	const FGameplayTag& PreviousInputTag
	)
{
	AbilityEquipped.Broadcast(AbilityTag, StatusTag, InputTag, PreviousInputTag);
}

FString UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return UAuraAbilitySystemLibrary::GetAbilityDescription(
			GetAvatarActor(),
			AbilityTag,
			AbilitySpec->Level
		);
	}

	return FString();
}

FString UAuraAbilitySystemComponent::GetNextLevelDescriptionByAbilityTag(
	const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return UAuraAbilitySystemLibrary::GetAbilityNextLevelDescription(
			GetAvatarActor(),
			AbilityTag,
			AbilitySpec->Level
		);
	}

	return FString();
}

FString UAuraAbilitySystemComponent::GetLockedDescriptionByAbilityTag(
	const FGameplayTag& AbilityTag)
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());
	const FAuraAbilityInfo& AuraAbilityInfo = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	
	return UAuraAbilitySystemLibrary::GetAbilityLockedDescription(
		AuraAbilityInfo.LevelRequirement,
		AuraAbilityInfo.AbilitiesRequirement
	);
}

void UAuraAbilitySystemComponent::SetInputTagFromSpec(
	FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& InputTag
	)
{
	SetExclusiveGameplayTagFromSpec(
		AbilitySpec,
		InputTag,
		FName("InputTag")
		);
}

void UAuraAbilitySystemComponent::RemoveInputTagFromAbilities(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	
	for (auto& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			Spec.DynamicAbilityTags.RemoveTag(InputTag);
			SetAbilityStatusFromSpec(Spec, FAuraGameplayTags::Get().Abilities_Status_Unlocked);
		}
	}
}

void UAuraAbilitySystemComponent::ServerSpendSkillPoint_Implementation(
	const FGameplayTag& AbilityTag
)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		
		FGameplayTag Status = GetAbilityStatusFromSpec(*AbilitySpec);
		
		if (Status.MatchesTagExact(Tags.Abilities_Status_Eligible))
		{
			Status = Tags.Abilities_Status_Unlocked;
			SetAbilityStatusFromSpec(*AbilitySpec, Status);
		}
		else if (
			Status.MatchesTagExact(Tags.Abilities_Status_Equipped) ||
			Status.MatchesTagExact(Tags.Abilities_Status_Unlocked)
			)
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityState(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);

		if (GetAvatarCharacter())
		{
			IPlayerInterface::Execute_SpendSkillPoints(AvatarCharacter, 1);
		}
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::EffectApplied(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle
)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityState_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	int32 AbilityLevel
)
{
	AbilityStateChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

