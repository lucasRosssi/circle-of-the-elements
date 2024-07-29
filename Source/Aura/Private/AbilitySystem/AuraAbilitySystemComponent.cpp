// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/ActiveAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/Aura.h"
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
	const TArray<TSubclassOf<UGameplayAbility>>& InAbilities,
	const FGameplayTag& AbilityStatus
)
{
	for (const auto AbilityClass : InAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const auto* ActiveAbility = Cast<UActiveAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ActiveAbility->StartupInputTag);
			SetAbilityStatusFromSpec(AbilitySpec, AbilityStatus);
		}
		GiveAbility(AbilitySpec);
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
	
	const ECharacterName CharacterName = GetAvatarCharacter()->GetCharacterName();
	RandomizedAbilitiesTagsLevel_1 = UAuraAbilitySystemLibrary::GetRandomAbilitiesFromLevel(
		GetAvatarActor(),
		CharacterName,
		1,
		MAX_AMOUNT_ABILITIES_PER_LEVEL
		);
	RandomizedAbilitiesTagsLevel_4 = UAuraAbilitySystemLibrary::GetRandomAbilitiesFromLevel(
		GetAvatarActor(),
		CharacterName,
		4,
		MAX_AMOUNT_ABILITIES_PER_LEVEL
		);
	RandomizedAbilitiesTagsLevel_8 = UAuraAbilitySystemLibrary::GetRandomAbilitiesFromLevel(
		GetAvatarActor(),
		CharacterName,
		8,
		MAX_AMOUNT_ABILITIES_PER_LEVEL
		);
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		AbilitySpecInputPressed(AbilitySpec);
		
		if (AbilitySpec.IsActive())
		{
			InvokeReplicatedEvent(
				EAbilityGenericReplicatedEvent::InputPressed,
				AbilitySpec.Handle,
				AbilitySpec.ActivationInfo.GetActivationPredictionKey()
				);
		}
		else if (const UActiveAbility* ActiveAbility = Cast<UActiveAbility>(AbilitySpec.Ability))
		{
			if (!ActiveAbility->bCanHoldInput) TryActivateAbility(AbilitySpec.Handle);
		}
		
		break;
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		if (const UActiveAbility* ActiveAbility = Cast<UActiveAbility>(AbilitySpec.Ability))
		{
			if (!ActiveAbility->bCanHoldInput) break;
		}
		
		AbilitySpecInputPressed(AbilitySpec);
		
		if (AbilitySpec.IsActive()) break;
		
		TryActivateAbility(AbilitySpec.Handle);
		break;
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		if (!AbilitySpec.IsActive()) break;
		
		AbilitySpecInputReleased(AbilitySpec);
		InvokeReplicatedEvent(
			EAbilityGenericReplicatedEvent::InputReleased,
			AbilitySpec.Handle,
			AbilitySpec.ActivationInfo.GetActivationPredictionKey()
			);
		break;
	}
}

void UAuraAbilitySystemComponent::ConfirmPressed()
{
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.IsActive()) continue;
		if (!Cast<UActiveAbility>(AbilitySpec.Ability)) continue;

		InputConfirm();
		break;
	}
}

void UAuraAbilitySystemComponent::CancelPressed()
{
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.IsActive()) continue;
		if (!Cast<UActiveAbility>(AbilitySpec.Ability)) continue;
		
		InputCancel();
		break;
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

TArray<FGameplayTag> UAuraAbilitySystemComponent::GetRandomizedAbilitiesFromLevel(int32 Level)
{
	switch (Level)
	{
	case 1:
		return RandomizedAbilitiesTagsLevel_1;
	case 4:
		return RandomizedAbilitiesTagsLevel_4;
	case 8:
		return RandomizedAbilitiesTagsLevel_8;
	default:
		return TArray<FGameplayTag>();
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
	const FGameplayTag& ParentTag
	)
{
	if (!GameplayTag.MatchesTag(ParentTag))
	{
		UE_LOG(LogAura, Error, TEXT(
			"Trying to set an invalid gameplay tag (%s)"
			" for the provided parent tag (%s)"
			),
			*GameplayTag.GetTagName().ToString(),
			*ParentTag.GetTagName().ToString()
			);
		return;	
	}
	
	if (AbilitySpec.DynamicAbilityTags.HasTag(ParentTag))
	{
		TArray<FGameplayTag> Tags;
		AbilitySpec.DynamicAbilityTags.GetGameplayTagArray(Tags);
		for (auto Tag : Tags)
		{
			if (Tag.MatchesTag(ParentTag))
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
		FAuraGameplayTags::Get().Abilities_Status
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
			const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
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

	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
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

FString UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(
	const UAbilityInfo* AbilityInfo,
	const FGameplayTag& AbilityTag
	)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return UAuraAbilitySystemLibrary::GetAbilityDescription(
			AbilityInfo,
			AbilityTag,
			AbilitySpec->Level
		);
	}

	return FString();
}

FString UAuraAbilitySystemComponent::GetNextLevelDescriptionByAbilityTag(
	const UAbilityInfo* AbilityInfo,
	const FGameplayTag& AbilityTag
	)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return UAuraAbilitySystemLibrary::GetAbilityNextLevelDescription(
			AbilityInfo,
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
		FAuraGameplayTags::Get().InputTag
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
	const FGameplayTag& AbilityTag,
	const FGameplayTag& InputTag
)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
		FGameplayTag Status = GetAbilityStatusFromSpec(*AbilitySpec);
		
		if (Status.MatchesTagExact(Tags.Abilities_Status_Eligible))
		{
			Status = Tags.Abilities_Status_Unlocked;
			SetAbilityStatusFromSpec(*AbilitySpec, Status);
			
			if (AbilityTag.MatchesTag(Tags.Abilities_Passive))
			{
				TryActivateAbility(AbilitySpec->Handle);
			}
		}
		else if (
			Status.MatchesTagExact(Tags.Abilities_Status_Equipped) ||
			Status.MatchesTagExact(Tags.Abilities_Status_Unlocked)
			)
		{
			AbilitySpec->Level += 1;
			if (UBaseAbility* BaseAbility = Cast<UBaseAbility>(AbilitySpec->Ability))
			{
				BaseAbility->OnAbilityLevelUp(AbilityActorInfo.Get(), AbilitySpec);
			}
		}
		ClientUpdateAbilityState(AbilityTag, Status, InputTag, AbilitySpec->Level);
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
	const FGameplayTag& InputTag,
	int32 AbilityLevel
)
{
	AbilityStateChanged.Broadcast(AbilityTag, StatusTag, InputTag, AbilityLevel);
}

