// Copyright Lucas Rossi


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/ActiveAbility.h"
#include "Aura/AuraLogChannels.h"
#include "Character/AuraCharacterBase.h"
#include "Game/AuraSaveGame.h"
#include "Interfaces/PlayerInterface.h"
#include "Utils/AuraSystemsLibrary.h"

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
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(ActiveAbility->StartupInputTag);
			SetAbilityStatusFromSpec(AbilitySpec, AbilityStatus);
		}
		GiveAbility(AbilitySpec);
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;
		
		AbilitySpecInputPressed(AbilitySpec);
		
		if (AbilitySpec.IsActive())
		{
		  if (const UGameplayAbility* Instance = AbilitySpec.GetPrimaryInstance())
		  {
			  InvokeReplicatedEvent(
				  EAbilityGenericReplicatedEvent::InputPressed,
				  AbilitySpec.Handle,
          Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
				  );
		  }
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
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;
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
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) continue;
		if (!AbilitySpec.IsActive()) break;
		
		AbilitySpecInputReleased(AbilitySpec);
	  if (const UGameplayAbility* Instance = AbilitySpec.GetPrimaryInstance())
	  {
	    InvokeReplicatedEvent(
        EAbilityGenericReplicatedEvent::InputReleased,
        AbilitySpec.Handle,
        Instance->GetCurrentActivationInfo().GetActivationPredictionKey()
        );
	  }
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
	
	if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(ParentTag))
	{
		TArray<FGameplayTag> Tags;
		AbilitySpec.GetDynamicSpecSourceTags().GetGameplayTagArray(Tags);
		for (auto Tag : Tags)
		{
			if (Tag.MatchesTag(ParentTag))
			{
				AbilitySpec.GetDynamicSpecSourceTags().RemoveTag(Tag);
			}
		}
	}
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(GameplayTag);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(
	const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (auto Tag : AbilitySpec.Ability.Get()->GetAssetTags())
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
	for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
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
	for (auto Tag : AbilitySpec.GetDynamicSpecSourceTags())
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

int32 UAuraAbilitySystemComponent::GetAbilityLevelFromTag(const FGameplayTag& AbilityTag)
{
  if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
  {
    return Spec->Level;
  }

  return 0;
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(
	const FGameplayTag& AbilityTag
	)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability.Get()->GetAssetTags().HasTagExact(AbilityTag))
		{
			return &AbilitySpec;
		}
	}

	return nullptr;
}

FGameplayEffectSpec UAuraAbilitySystemComponent::GetEffectSpecByTag(const FGameplayTag& EffectTag)
{
  const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(FGameplayTagContainer({ EffectTag }));
  const TArray<FActiveGameplayEffectHandle> Effects = GetActiveEffects(Query);

  if (Effects.IsEmpty()) return FGameplayEffectSpec();

  const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Effects[0]);
  if (!ActiveEffect) return FGameplayEffectSpec();

  return ActiveEffect->Spec;
}

void UAuraAbilitySystemComponent::AddAttribute(const FGameplayTag& AttributeTag, int32 Amount)
{
  FGameplayEventData Payload;
  Payload.EventTag = AttributeTag;
  Payload.EventMagnitude = Amount;

  UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
    GetAvatarActor(),
    AttributeTag,
    Payload
  );

  if (!GetSaveGame()) return;
  
  if (float* Attribute = SaveGame->AttributeSet.Attributes.Find(AttributeTag))
  {
    *Attribute += Amount;
  }
  else
  {
    SaveGame->AttributeSet.Attributes.Add(AttributeTag, 10 + Amount);
  }
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
	AddAttribute(AttributeTag);

	IPlayerInterface::Execute_AddAttributePoints(GetAvatarActor(), -1);
}

// void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
// {
// 	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilitiesInfo(GetAvatarActor());
//
// 	for (
// 		const auto& Info :
// 		AbilityInfo->FindAbilitiesFromCharacter(GetAvatarCharacter()->GetCharacterName()))
// 	{
// 		if (!Info.AbilityTag.IsValid()) continue;
// 		if (Level < Info.LevelRequirement) continue;
// 		
// 		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
// 		{
// 			const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
// 			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
// 			SetAbilityStatusFromSpec(AbilitySpec, Tags.Abilities_Status_Eligible);
// 			GiveAbility(AbilitySpec);
// 			MarkAbilitySpecDirty(AbilitySpec);
// 			ClientUpdateAbilityState(Info.AbilityTag, Tags.Abilities_Status_Eligible);
// 		}
// 	}
// }

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

void UAuraAbilitySystemComponent::UnlockAbility(FGameplayAbilitySpec& AbilitySpec)
{
	if (!GetAvatarActor()->HasAuthority()) return;
	SetAbilityStatusFromSpec(AbilitySpec, FAuraGameplayTags::Get().Abilities_Status_Unlocked);
	GiveAbility(AbilitySpec);
	MarkAbilitySpecDirty(AbilitySpec);
}

FString UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(
	const UAbilityInfo* AbilityInfo,
	const FGameplayTag& AbilityTag,
	bool bNextLevel
	)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return UAuraAbilitySystemLibrary::GetAbilityDescription(
			AbilityInfo,
			AbilityTag,
			AbilitySpec->Level,
			bNextLevel
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

bool UAuraAbilitySystemComponent::IsInputTagAssigned(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return false;

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)) return true;
	}

	return false;
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
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			Spec.GetDynamicSpecSourceTags().RemoveTag(InputTag);
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
			IPlayerInterface::Execute_AddSkillPoints(AvatarCharacter, -1);
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

UAuraSaveGame* UAuraAbilitySystemComponent::GetSaveGame()
{
  if (SaveGame == nullptr)
  {
    SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(GetAvatarActor());
  }

  return SaveGame;
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

