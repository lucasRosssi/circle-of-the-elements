// Copyright Lucas Rossi


#include "Game/Components/AbilityManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Enums/CharacterName.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystem/Abilities/BaseAbility.h"

UAbilityManager::UAbilityManager()
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	
	FGameplayTagContainer TierTags;
 	for (const FGameplayTag& TierTag : *Tags.ParentsToChildren.Find(Tags.Abilities_Tier))
	{
		TierTags.AddTagFast(TierTag);
	}

	for (const FGameplayTag& ElementTag : *Tags.ParentsToChildren.Find(Tags.Abilities_Element))
	{
		ElementalTierPool.Add(ElementTag, TierTags);
		AbilitiesBag.Add(ElementTag);
	}
}

void UAbilityManager::BeginPlay()
{
	Super::BeginPlay();

	AssignNextAbilities();
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

void UAbilityManager::AssignNextAbilities()
{
	const ECharacterName CharacterName = GetAuraGameMode()->CurrentCharacterName;
	
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	for (const FGameplayTag& ElementTag : *Tags.ParentsToChildren.Find(Tags.Abilities_Element))
	{
		RandomizeElementAbilities(CharacterName, ElementTag);
	}
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

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityManager::GetRemainingElementAbilities(
	ECharacterName CharacterName,
	const FGameplayTag ElementTag
	)
{
	return GetElementAbilities(CharacterName, ElementTag).FilterByPredicate(
		[this](const TTuple<FGameplayTag, FAuraAbilityInfo>& 
	AbilityInfo)
	{
		return !AcquiredAbilities.HasTagExact(AbilityInfo.Key) ||
			!BlockedAbilities.HasTagExact(AbilityInfo.Key);
	});
}

TArray<FAuraAbilityInfo> UAbilityManager::GetRemainingTierAbilities(
	const FGameplayTag& TierTag,
	const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities
	)
{
	TArray<FAuraAbilityInfo> Abilities;
	ElementAbilities.GenerateValueArray(Abilities);
	Abilities.FilterByPredicate([this, TierTag](const FAuraAbilityInfo& Ability)
		{
			return TierTag.MatchesTagExact(Ability.TierTag);
		});
	
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

TMap<FGameplayTag, int32> UAbilityManager::GetAvailableTiers(const FGameplayTag& ElementTag)
{
	const FGameplayTagContainer* TierPool = ElementalTierPool.Find(ElementTag);

	if (TierPool == nullptr) return TMap<FGameplayTag, int32>();
	
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
	
	TMap<FGameplayTag, int32> AvailableTiers = GetAvailableTiers(ElementTag);

	if (AvailableTiers.IsEmpty()) return TArray<FAuraAbilityInfo>();

	TMap<FGameplayTag, FAuraAbilityInfo> ElementAbilities =	GetRemainingElementAbilities(CharacterName, ElementTag);
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
		
		AbilitiesBag.Find(ElementTag)->AddTag(RemainingAbilities[RandomAbilityIndex].AbilityTag);
	}
	
	return SelectedAbilities;
}

TArray<FAuraAbilityInfo> UAbilityManager::GetAbilitiesFromBag(const FGameplayTag& ElementTag)
{
	TArray<FAuraAbilityInfo> AbilitiesInfos;
	if (const FGameplayTagContainer* AbilitiesTags = AbilitiesBag.Find(ElementTag))
	{
		for (auto AbilityTagIterator = AbilitiesTags->CreateConstIterator(); AbilityTagIterator; ++AbilityTagIterator)
		{
			FAbilityInfoParams Params;
			Params.ElementTag = ElementTag;
			Params.CharacterName = GetAuraGameMode()->CurrentCharacterName;
			Params.AbilityTag = *AbilityTagIterator;
			AbilitiesInfos.Add(GetAbilityInfo(Params));
		}
	}

	return AbilitiesInfos;
}

void UAbilityManager::GetAbilityFormattedTexts(
		const FAuraAbilityInfo& AbilityInfo,
		FText& AbilityName,
		FText& AbilityDescription,
		FText& AbilityDetails
		)
{
	const UBaseAbility* BaseAbility = AbilityInfo.Ability.GetDefaultObject();

	AbilityName = FText::FromString(FString::Printf(
		TEXT("<Title>%s</>"),
		*AbilityInfo.Name.ToString()
		));

	AbilityDescription = AbilityInfo.Description;
	UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
		BaseAbility,
		1,
		AbilityDescription
		);

	FString ManaCostText;
	FString CooldownText;
	UAuraAbilitySystemLibrary::MakeManaAndCooldownText(
		BaseAbility,
		1,
		ManaCostText,
		CooldownText
		);
	AbilityDetails = FText::FromString(FString::Printf(TEXT(
			"%s"
			"%s"
			),
		*ManaCostText,
		*CooldownText
	));
}
