// Copyright Lucas Rossi


#include "Game/Components/AbilityManager.h"

#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"
#include "Enums/CharacterName.h"
#include "Game/AuraGameModeBase.h"

UAbilityManager::UAbilityManager()
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	FGameplayTagContainer TierTags;
	TierTags.AddTagFast(Tags.Abilities_Tier_I);
	TierTags.AddTagFast(Tags.Abilities_Tier_II);
	TierTags.AddTagFast(Tags.Abilities_Tier_III);
	TierTags.AddTagFast(Tags.Abilities_Tier_IV);
	TierTags.AddTagFast(Tags.Abilities_Tier_V);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Physical,
		TierTags
		);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Arcane,
		TierTags
		);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Fire,
		TierTags
		);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Ice,
		TierTags
		);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Lightning,
		TierTags
		);
	ElementalTierPool.Add(
		Tags.Abilities_Element_Necrotic,
		TierTags
		);
}

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityManager::GetElementAbilities(
	ECharacterName CharacterName,
	const FGameplayTag ElementTag
	)
{
	return GetAuraGameMode()->AbilityInfo->FindCharacterAbilitiesOfElement(CharacterName,	ElementTag);
}

FAuraAbilityInfo UAbilityManager::GetAbilityInfo(const FAbilityInfoParams& Params)
{
	return GetAuraGameMode()->AbilityInfo->FindAbilityInfoWithParams(Params);
}

FGameplayTag UAbilityManager::RandomizeTier(const TMap<FGameplayTag, int32>& TierMap)
{
	float TiersSum = 0.f;
	for (const auto& Tier : TierMap)
	{
		TiersSum += Tier.Value;
	}
	float RandomFloat = FMath::FRandRange(0.f, TiersSum);
	for (const auto& Tier : TierMap)
	{
		if (Tier.Value < RandomFloat)
		{
			return Tier.Key;
		}

		RandomFloat -= Tier.Value;
	}

	return FGameplayTag();
}

TArray<FAuraAbilityInfo> UAbilityManager::GetRemainingTierAbilities(
	const FGameplayTag& TierTag,
	const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities
	)
{
	TArray<FAuraAbilityInfo> Abilities;
	ElementAbilities
		.FilterByPredicate([this, TierTag](const TTuple<FGameplayTag, FAuraAbilityInfo>& Ability)
		{
			return TierTag.MatchesTagExact(Ability.Value.TierTag)
				&& !Ability.Key.MatchesAnyExact(AcquiredAbilities)
				&& !Ability.Key.MatchesAnyExact(BlockedAbilities);
		}).GenerateValueArray(Abilities);
	
	if (Abilities.IsEmpty())
	{
		UE_LOG(
			LogAura,
			Warning,
			TEXT("No Ability of tier %s left!"),
			*TierTag.ToString()
			);
	}

	return Abilities;
}

TMap<FGameplayTag, int32> UAbilityManager::GetAvailableTiers(const FGameplayTagContainer* TierPool)
{
	return GetAuraGameMode()->AbilityInfo->TierDropProbability
	  .FilterByPredicate([TierPool](const TTuple<FGameplayTag, int32>& TierInfo)
	  {
	    return TierPool->HasTagExact(TierInfo.Key);
	  });
}

TArray<FAuraAbilityInfo> UAbilityManager::RandomizeElementAbilities(
	ECharacterName CharacterName,
	const FGameplayTag& ElementTag,
	int32 Amount
	)
{
	check(Amount > 0);

	const FGameplayTagContainer* TierPool = ElementalTierPool.Find(ElementTag);
	if (TierPool == nullptr) return TArray<FAuraAbilityInfo>();
	
	TMap<FGameplayTag, int32> AvailableTiers = GetAvailableTiers(TierPool);

	if (AvailableTiers.IsEmpty()) return TArray<FAuraAbilityInfo>();

	TMap<FGameplayTag, FAuraAbilityInfo> ElementAbilities =	GetElementAbilities(CharacterName, ElementTag);
	TArray<FAuraAbilityInfo> SelectedAbilities;
	for (int32 i = 0; i < Amount; i++)
	{
		const FGameplayTag& SelectedTierTag = RandomizeTier(AvailableTiers);
		const TArray<FAuraAbilityInfo>& RemainingAbilities = GetRemainingTierAbilities(SelectedTierTag, ElementAbilities);

		if (RemainingAbilities.IsEmpty())
		{
			AvailableTiers.Remove(SelectedTierTag);
			ElementalTierPool.Find(ElementTag)->RemoveTag(SelectedTierTag);
			if (AvailableTiers.IsEmpty()) break;
			continue;
		}

		const int32 RandomAbilityIndex = FMath::RandRange(0, RemainingAbilities.Num() - 1);
		
		SelectedAbilities.Add(RemainingAbilities[RandomAbilityIndex]);
		ElementAbilities.Remove(RemainingAbilities[RandomAbilityIndex].AbilityTag);
	}

	return SelectedAbilities;
}
