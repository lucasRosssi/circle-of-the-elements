// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/BaseAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/StatusEffectInfo.h"
#include "AbilitySystem/GameplayEffects/StatusEffect.h"
#include "Kismet/KismetMathLibrary.h"

float UBaseAbility::GetManaCost(int32 Level) const
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

float UBaseAbility::GetCooldown(int32 Level) const
{
	return Cooldown.GetValueAtLevel(Level);
}

int32 UBaseAbility::GetRoundedManaCost(int32 Level) const
{
	return FMath::RoundToInt32(GetManaCost(Level));
}

int32 UBaseAbility::GetRoundedCooldown(int32 Level) const
{
	return FMath::RoundToInt32(GetCooldown(Level));
}

FAbilityParams UBaseAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
	FAbilityParams AbilityParams;
	AbilityParams.WorldContextObject = GetAvatarActorFromActorInfo();
	AbilityParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	AbilityParams.TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	AbilityParams.AbilityLevel = GetAbilityLevel();

	if (bIsAreaAbility)
	{
		AbilityParams.bIsAreaAbility = true;
		AbilityParams.AreaInnerRadius = AreaInnerRadius;
		AbilityParams.AreaOuterRadius = AreaOuterRadius;
		AbilityParams.AreaOrigin = AreaOrigin;
		if (IsValid(TargetActor))
		{
			const FVector ActorLocation = TargetActor->GetActorLocation();
			AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
				AreaOrigin, 
				ActorLocation
				);
		}
	}
	
	if (StatusEffectData.IsValid())
	{
		UStatusEffectInfo* StatusEffectInfo = UAuraAbilitySystemLibrary
			::GetStatusEffectInfo(AbilityParams.WorldContextObject);
		AbilityParams.EffectParams.GameplayEffectClass = StatusEffectInfo
			->StatusEffects.Find(StatusEffectData.StatusEffectTag)->StatusEffectClass;
		AbilityParams.EffectParams.GameplayTag = StatusEffectData.StatusEffectTag;
		AbilityParams.EffectParams.Value = StatusEffectData.Value;
		AbilityParams.EffectParams.Duration = StatusEffectData.Duration;
	}
	
	return AbilityParams;
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
