// Copyright Lucas Rossi


#include "UI/WidgetController/SkillMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USkillMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SkillPointsChanged.Broadcast(GetAuraPlayerState()->GetSkillPoints());
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStateChanged.AddLambda(
		[this](
			const FGameplayTag& AbilityTag,
			const FGameplayTag& StatusTag,
			int32 NewLevel
			)
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);

	GetAuraPlayerState()->OnSkillPointsChangedDelegate.AddLambda(
		[this](int32 SkillPoints)
		{
			SkillPointsChanged.Broadcast(SkillPoints);
		}
	);
}

void USkillMenuWidgetController::SkillGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const int32 SkillPoints = GetAuraPlayerState()->GetSkillPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_NONE);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag
	(AbilityTag);
	if (bTagValid && !bTagNone && AbilitySpec)
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetAbilityStatusFromSpec(*AbilitySpec);
	}
	else
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}

	FString Description;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		Description = GetAuraAbilitySystemComponent()
			->GetLockedDescriptionByAbilityTag(AbilityTag);
	}
	else if (
		AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible) ||
		SkillPoints == 0
		)
	{
		Description = GetAuraAbilitySystemComponent()
			->GetDescriptionByAbilityTag(AbilityInfo, AbilityTag);
	}
	else
	{
		Description = GetAuraAbilitySystemComponent()
			->GetNextLevelDescriptionByAbilityTag(AbilityInfo, AbilityTag);
	}

	bool bEnableSpendPoint;
	bool bEnableEquip;
	ShouldEnableInteractions(AbilityStatus,	SkillPoints, bEnableSpendPoint, bEnableEquip);
	SkillGlobeSelectedDelegate.Broadcast(bEnableSpendPoint, bEnableEquip, Description);
}

void USkillMenuWidgetController::SpendPoint(FGameplayTag AbilityTag)
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSkillPoint(AbilityTag);
	}
}

FString USkillMenuWidgetController::GetSkillDescription(FGameplayTag AbilityTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const int32 SkillPoints = GetAuraPlayerState()->GetSkillPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(GameplayTags.Abilities_NONE);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()
		->GetSpecFromAbilityTag(AbilityTag);
	if (bTagValid && !bTagNone && AbilitySpec)
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetAbilityStatusFromSpec(*AbilitySpec);
	}
	else
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}

	FString Description;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		Description = GetAuraAbilitySystemComponent()
			->GetLockedDescriptionByAbilityTag(AbilityTag);
	}
	else if (
		AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible) ||
		SkillPoints == 0 // Unlocked or equipped, but can't level up
		)
	{
		Description = GetAuraAbilitySystemComponent()
			->GetDescriptionByAbilityTag(AbilityInfo, AbilityTag);
	}
	else // Unlocked or equipped and can level up
	{
		Description = GetAuraAbilitySystemComponent()
			->GetNextLevelDescriptionByAbilityTag(AbilityInfo, AbilityTag);
	}

	return Description;
}

void USkillMenuWidgetController::EquipToInputPressed(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& InputTag
	)
{
	GetAuraAbilitySystemComponent()->ServerEquipAbility(AbilityTag, InputTag);
}

void USkillMenuWidgetController::ShouldEnableInteractions(
	const FGameplayTag& AbilityStatus,
	int32 SkillPoints,
	bool& bShouldEnableSpendPoint,
	bool& bShouldEnableEquip
)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquip = true;
		bShouldEnableSpendPoint = SkillPoints > 0;
	} else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquip = true;
		bShouldEnableSpendPoint = SkillPoints > 0;
	} else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquip = false;
		bShouldEnableSpendPoint = SkillPoints > 0;
	} else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bShouldEnableEquip = false;
		bShouldEnableSpendPoint = false;
	}
}
