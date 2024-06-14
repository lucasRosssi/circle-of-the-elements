// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/BaseAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

float UBaseAbility::GetManaCost(int32 Level)
{
	float ManaCost = 0.f;
	if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (auto Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(
					Level,
					ManaCost
					);
				break;
			}
		}
	}

	return FMath::Abs(ManaCost);
}

float UBaseAbility::GetCooldown()
{
	return BaseCooldown;
}

int32 UBaseAbility::GetRoundedManaCost(int32 Level)
{
	return FMath::RoundToInt32(GetManaCost(Level));
}

int32 UBaseAbility::GetRoundedCooldown()
{
	return FMath::RoundToInt32(GetCooldown());
}

FGameplayTag UBaseAbility::GetAbilityTag()
{
	if (AbilityTag.IsValid()) return AbilityTag;
	
	TArray<FGameplayTag> GameplayTags;
	AbilityTags.GetGameplayTagArray(GameplayTags);
	for (auto Tag : GameplayTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			AbilityTag = Tag;
			break;
		}
	}

	return AbilityTag;
}

void UBaseAbility::FormatDescriptionAtLevel(FText& Description,  int32 Level)
{
	
}
