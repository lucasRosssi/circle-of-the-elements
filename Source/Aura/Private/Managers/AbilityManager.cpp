// Copyright Lucas Rossi


#include "Managers/AbilityManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "AbilitySystem/Abilities/BaseAbility.h"
#include "Game/AuraSaveGame.h"
#include "Managers/MatchManager.h"
#include "Managers/RewardManager.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"

UAbilityManager::UAbilityManager()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  FGameplayTagContainer TierTags;
  for (const FGameplayTag& TierTag : *AuraTags.ParentsToChildren.Find(AuraTags.Abilities_Tier))
  {
    TierTags.AddTagFast(TierTag);
  }

  for (const FGameplayTag& ElementTag : *AuraTags.ParentsToChildren.Find(AuraTags.Abilities_Element))
  {
    ElementalTierPool.Add(ElementTag, TierTags);
    AbilitiesContainer.Add(ElementTag);
  }
}

void UAbilityManager::GiveAcquiredAbilities(AActor* Actor)
{
  if (!AcquiredAbilities.IsEmpty())
  {
    UE_LOG(
      LogAura,
      Warning,
      TEXT("Ability Manager is overriding Abilities! If it is not the intention, check it in the game mode.")
    );
  }

  // const UAbilityInfo* AbilitiesDataAsset = UAuraSystemsLibrary::GetAbilitiesInfo(this);
  AAuraPlayerState* AuraPS = UAuraSystemsLibrary::GetAuraPlayerState(GetOwner());

  if (!AuraPS) return;

  int32 i = 0;
  for (const auto& [AbilityTag, Level] : AcquiredAbilities)
  {
    USpirit* Spirit = NewObject<USpirit>();
    Spirit->SetAbilityTag(AbilityTag);
    Spirit->SetID(FGuid::NewGuid());
    Spirit->SetLevel(Level);
    AuraPS->AddEquipmentToInventory(Spirit);
    Spirit->Equip(i++);
    // OnAbilitySelectedDelegate.Broadcast(AbilityInfo);
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
  const FGameplayTag ElementTag
)
{
  return UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindElementAbilities(ElementTag);
}

FAuraAbilityInfo UAbilityManager::GetAbilityInfo(const FAbilityInfoParams& Params)
{
  return UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindAbilityInfoWithParams(Params);
}

FAuraAbilityInfo UAbilityManager::FindAbilityInfoByTag(const FGameplayTag& AbilityTag)
{
  return UAuraSystemsLibrary::GetAbilitiesInfo(this)->FindAbilityInfoByTag(AbilityTag);
}

TArray<FAuraAbilityInfo> UAbilityManager::RandomizeElementAbilities(
  const FGameplayTag& ElementTag,
  int32 Amount
)
{
  check(Amount > 0);

  TMap<FGameplayTag, int32> AvailableTiers = GetAvailableTiers(ElementTag);

  if (AvailableTiers.IsEmpty()) return TArray<FAuraAbilityInfo>();

  TMap<FGameplayTag, FAuraAbilityInfo> ElementAbilities = GetRemainingElementAbilities(ElementTag);
  TArray<FAuraAbilityInfo> SelectedAbilities;
  while (SelectedAbilities.Num() < Amount && !AvailableTiers.IsEmpty())
  {
    const FGameplayTag& SelectedTierTag = RandomizeTier(AvailableTiers);
    const TArray<FAuraAbilityInfo>& RemainingAbilities = GetRemainingTierAbilities(SelectedTierTag, ElementAbilities);

    if (RemainingAbilities.IsEmpty())
    {
      AvailableTiers.Remove(SelectedTierTag);
      if (FGameplayTagContainer* TierContainer = ElementalTierPool.Find(ElementTag))
      {
        TierContainer->RemoveTag(SelectedTierTag);
      }
      continue;
    }

    const int32 RandomAbilityIndex = FMath::RandRange(0, RemainingAbilities.Num() - 1);
    const FAuraAbilityInfo& ChosenAbility = RemainingAbilities[RandomAbilityIndex];

    SelectedAbilities.Add(ChosenAbility);
    ElementAbilities.Remove(ChosenAbility.AbilityTag);

    if (FGameplayTagContainer* AbilitiesTagContainer = AbilitiesContainer.Find(ElementTag))
    {
      AbilitiesTagContainer->AddTag(ChosenAbility.AbilityTag);
    }
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
      Params.AbilityTag = *AbilityTagIterator;
      AbilitiesInfos.Add(GetAbilityInfo(Params));
    }
  }

  return AbilitiesInfos;
}

void UAbilityManager::GetAbilityFormattedTexts(
  const FAuraAbilityInfo& AbilityInfo,
  bool bNextLevel,
  FText& AbilityName,
  FText& AbilityDescription,
  FText& AbilityDetails
)
{
  const UBaseAbility* BaseAbility = AbilityInfo.Ability.GetDefaultObject();

  AbilityName = FText::FromString(
    FString::Printf(
      TEXT("<Title>%s</>"),
      *AbilityInfo.Name.ToString()
    )
  );

  AbilityDescription = AbilityInfo.Description;
  UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
    AbilityInfo,
    1,
    AbilityDescription,
    bNextLevel
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
  AbilityDetails = FText::FromString(
    FString::Printf(
      TEXT(
        "%s"
        "%s"
        "%s"
      ),
      *ManaCostText,
      *ChargesText,
      *CooldownText
    )
  );
}

void UAbilityManager::SelectAbilityReward(
  const FGameplayTag& ElementTag,
  const FAuraAbilityInfo& AbilityInfo
)
{
  // AcquiredAbilities.Add(AbilityInfo.AbilityTag, Level);

  USpirit* Spirit = NewObject<USpirit>();
  Spirit->SetAbilityTag(AbilityInfo.AbilityTag);
  Spirit->SetElementTag(ElementTag);
  Spirit->Spawn(GetOwner());

  UAuraSystemsLibrary::GetMatchManager(GetOwner())->RegisterLoot(Spirit);

  if (!bOverrideAbilitiesContainer)
  {
    AbilitiesContainer[ElementTag].Reset();
    RandomizeElementAbilities(
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
    RandomizeElementAbilities(ElementTag);
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
    if (Tier.Value > RandomFloat)
    {
      return Tier.Key;
    }

    RandomFloat -= Tier.Value;
  }

  return FGameplayTag();
}

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityManager::GetRemainingElementAbilities(
  const FGameplayTag ElementTag
)
{
  return GetElementAbilities(ElementTag).FilterByPredicate(
    [this](
    const TTuple<FGameplayTag, FAuraAbilityInfo>&
    AbilityInfo
  )
    {
      return !AcquiredAbilities.Contains(AbilityInfo.Key) &&
        !BlockedAbilities.HasTagExact(AbilityInfo.Key);
    }
  );
}

TArray<FAuraAbilityInfo> UAbilityManager::GetRemainingTierAbilities(
  const FGameplayTag& TierTag,
  const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities
)
{
  TArray<FAuraAbilityInfo> Abilities;
  ElementAbilities.GenerateValueArray(Abilities);
  TArray<FAuraAbilityInfo> FilteredAbilities = Abilities.FilterByPredicate(
    [this, TierTag](const FAuraAbilityInfo& Ability)
    {
      return TierTag.MatchesTagExact(Ability.TierTag);
    }
  );

  if (FilteredAbilities.IsEmpty())
  {
    UE_LOG(
      LogAura,
      Warning,
      TEXT("No Ability of tier %s left!"),
      *TierTag.ToString()
    );
  }

  return FilteredAbilities;
}

TMap<FGameplayTag, int32> UAbilityManager::GetAvailableTiers(const FGameplayTag& ElementTag)
{
  const FGameplayTagContainer* TierPool = ElementalTierPool.Find(ElementTag);

  if (TierPool == nullptr) return TMap<FGameplayTag, int32>();

  return UAuraSystemsLibrary::GetAbilitiesInfo(this)->TierDropProbability
                                                    .FilterByPredicate(
                                                      [TierPool](
                                                      const TTuple<FGameplayTag, int32>& TierInfo
                                                    )
                                                      {
                                                        return TierPool->HasTagExact(TierInfo.Key);
                                                      }
                                                    );
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

FGameplayAbilitySpec UAbilityManager::GiveAbility(
  UAuraAbilitySystemComponent* AuraASC,
  const FAuraAbilityInfo& AbilityInfo,
  int32 Level,
  const FGameplayTag& InputTag
)
{
  FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.Ability, Level);
  AuraASC->UnlockAbility(AbilitySpec);

  if (IAbilityInterface::Execute_IsActiveAbility(AbilitySpec.Ability))
  {
    if (InputTag.IsValid())
    {
      AuraASC->ServerEquipAbility(AbilityInfo.AbilityTag, InputTag);
      return AbilitySpec;
    }

    const FGameplayTag& AvailableInputTag = GetAvailableInputTag(AuraASC);
    if (AvailableInputTag.IsValid())
    {
      AuraASC->ServerEquipAbility(AbilityInfo.AbilityTag, AvailableInputTag);
    }
  }

  return AbilitySpec;
}

void UAbilityManager::RemoveAbility(UAuraAbilitySystemComponent* AuraASC, const FGameplayTag& AbilityTag)
{
  const FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(AbilityTag);
  if (!AbilitySpec) return;

  AuraASC->ClearAbility(AbilitySpec->Handle);
}
