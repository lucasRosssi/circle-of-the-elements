// Copyright Lucas Rossi


#include "Managers/AbilityManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Enums/CharacterName.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "Managers/RewardManager.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"

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
		AbilitiesContainer.Add(ElementTag);
	}
}

void UAbilityManager::GiveAcquiredAbilities(AActor* Actor)
{
  if (AcquiredAbilities.Num() > 0)
  {
    for (auto AbilityTagIterator = AcquiredAbilities.CreateConstIterator(); AbilityTagIterator; ++AbilityTagIterator)
    {
      const FAuraAbilityInfo& AbilityInfo = UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindAbilityInfoByTag(*AbilityTagIterator);
      UAuraAbilitySystemComponent* AuraASC = UAuraAbilitySystemLibrary::GetAuraAbilitySystemComponent(Actor);
      GiveAbility(AbilityInfo, AuraASC);
    }
  }
}

void UAbilityManager::BeginPlay()
{
	Super::BeginPlay();

  if (!bInitializeOnBeginPlay) return;

  if (bOverrideAbilitiesContainer)
  {
    OverridenAbilitiesContainer = AbilitiesContainer;
    return;
  }

	AssignInitialAbilities();
}

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityManager::GetElementAbilities(
	ECharacterName CharacterName,
	const FGameplayTag ElementTag
	)
{
	return UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindCharacterAbilitiesOfElement(CharacterName,	ElementTag);
}

FAuraAbilityInfo UAbilityManager::GetAbilityInfo(const FAbilityInfoParams& Params)
{
	return UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindAbilityInfoWithParams(Params);
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
		
		AbilitiesContainer.Find(ElementTag)->AddTag(RemainingAbilities[RandomAbilityIndex].AbilityTag);
	}
	
	return SelectedAbilities;
}

TArray<FAuraAbilityInfo> UAbilityManager::GetAbilitiesFromBag(const FGameplayTag& ElementTag)
{
	TArray<FAuraAbilityInfo> AbilitiesInfos;
	if (const FGameplayTagContainer* AbilitiesTags = AbilitiesContainer.Find(ElementTag))
	{
		for (auto AbilityTagIterator = AbilitiesTags->CreateConstIterator(); AbilityTagIterator; ++AbilityTagIterator)
		{
			FAbilityInfoParams Params;
			Params.ElementTag = ElementTag;
			Params.HeroName = GetHeroName();
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
		AbilityInfo,
		1,
		AbilityDescription
		);

	FString ManaCostText;
	FString CooldownText;
	FString ChargesText;
	UAuraAbilitySystemLibrary::MakeAbilityDetailsText(
		BaseAbility,
		1,
		ManaCostText,
		CooldownText,
		ChargesText
		);
	AbilityDetails = FText::FromString(FString::Printf(TEXT(
			"%s"
			"%s"
			"%s"
			),
		*ManaCostText,
		*ChargesText,
		*CooldownText
	));
}



void UAbilityManager::SelectAbilityReward(
		const FGameplayTag& ElementTag,
		const FAuraAbilityInfo& AbilityInfo,
		UAuraAbilitySystemComponent* AuraASC
		)
{
	GiveAbility(AbilityInfo, AuraASC);
	AcquiredAbilities.AddTag(AbilityInfo.AbilityTag);

  if (!bOverrideAbilitiesContainer)
  {
	  AbilitiesContainer[ElementTag].Reset();
	  RandomizeElementAbilities(
		  GetHeroName(),
		  ElementTag
		  );
  }
	
	OnAbilitySelectedDelegate.Broadcast(AbilityInfo);
	UAuraAbilitySystemLibrary::GetOverlayWidgetController(GetOwner())
		->AbilityInfoDelegate.Broadcast(AbilityInfo);

  if (AbilitiesContainer[ElementTag].IsEmpty())
  {
    OnNoAbilitiesLeft(ElementTag);
  }
}

void UAbilityManager::AssignInitialAbilities()
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	for (const FGameplayTag& ElementTag : Tags.ParentsToChildren[Tags.Abilities_Element])
	{
		RandomizeElementAbilities(GetHeroName(), ElementTag);
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
		return !AcquiredAbilities.HasTagExact(AbilityInfo.Key) &&
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
	
	return UAuraSystemsLibrary::GetAbilitiesInfo(this)->TierDropProbability
	  .FilterByPredicate([TierPool](const TTuple<FGameplayTag, int32>& TierInfo)
	  {
	    return TierPool->HasTagExact(TierInfo.Key);
	  });
}

void UAbilityManager::OnNoAbilitiesLeft(const FGameplayTag& ElementTag)
{
  if (const FGameplayTag* EssenceTag = FAuraGameplayTags::Get().EssenceToAbility.FindKey(ElementTag))
  {
    URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
    if (RewardManager)
    {
      RewardManager->RemoveRewardFromPool(*EssenceTag);
    }
  }
  else
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Matching Essence Tag for %s not found! Failed to remove essence from the reward pool."),
      *ElementTag.ToString()
      )
  }
}

FGameplayTag UAbilityManager::GetAvailableInputTag(UAuraAbilitySystemComponent* AuraASC)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	for (const auto& InputTag : AuraTags.ParentsToChildren[AuraTags.InputTag])
	{
		if (!AuraASC->IsInputTagAssigned(InputTag)) return InputTag;
	}

	return FGameplayTag();
}

void UAbilityManager::GiveAbility(const FAuraAbilityInfo& AbilityInfo, UAuraAbilitySystemComponent* AuraASC)
{
  FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.Ability, 1);
  AuraASC->UnlockAbility(AbilitySpec);
	
  if (IAbilityInterface::Execute_IsActiveAbility(AbilitySpec.Ability))
  {
    const FGameplayTag& AvailableInputTag = GetAvailableInputTag(AuraASC);
    if (AvailableInputTag.IsValid())
    {
      AuraASC->ServerEquipAbility(AbilityInfo.AbilityTag, AvailableInputTag);
    }
  }
}

ECharacterName UAbilityManager::GetHeroName()
{
  if (HeroName == ECharacterName::Undefined)
  {
    HeroName = UAuraSystemsLibrary::GetCurrentHeroData(this).HeroName;
  }

  return HeroName;
}
