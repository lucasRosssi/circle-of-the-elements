// Copyright Lucas Rossi


#include "Managers/UpgradeManager.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/UpgradeAbility.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "AbilitySystem/GameplayEffects/UpgradeEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"

void UUpgradeManager::GiveAcquiredUpgrades()
{
  if (AcquiredUpgrades.Num() > 0)
  {
    for (auto UpgradeTagIterator = AcquiredUpgrades.CreateConstIterator(); UpgradeTagIterator; ++UpgradeTagIterator)
    {
      UnlockUpgrade(*UpgradeTagIterator);
    }
  }
}

TMap<FGameplayTag, FAuraUpgradeInfo> UUpgradeManager::GetElementUpgrades(
  ECharacterName CharacterName,
  const FGameplayTag ElementTag
)
{
  return UpgradeInfo->FindCharacterUpgradesOfElement(CharacterName, ElementTag);
}

FAuraUpgradeInfo UUpgradeManager::GetUpgradeInfo(const FUpgradeInfoParams& Params)
{
  return UpgradeInfo->FindUpgradeInfoWithParams(Params);
}

void UUpgradeManager::GetUpgradeFormattedTexts(
  const FAuraUpgradeInfo& AuraUpgradeInfo,
  int32 Level,
  bool bNextLevel,
  FText& UpgradeName,
  FText& UpgradeDescription,
  FText& UpgradeDetails
)
{
  UpgradeName = FText::FromString(FString::Printf(
    TEXT("<Title>%s</>\n\n"),
    *AuraUpgradeInfo.Name.ToString()
    ));

  UpgradeDescription = bNextLevel ? AuraUpgradeInfo.NextLevelDescription : AuraUpgradeInfo.Description;
  const UUpgradeAbility* Ability = AuraUpgradeInfo.Ability.GetDefaultObject();
  UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
    Ability,
    Level,
    UpgradeDescription
    );

  FString CostText;
  FString RequirementsText;
  UAuraAbilitySystemLibrary::MakeUpgradeDetailsText(
    Ability,
    Level,
    CostText,
    RequirementsText
    );

  if (AuraUpgradeInfo.Requirements.Num() > 0)
  {
    UpgradeDetails = FText::FromString(FString::Printf(TEXT(
        "\n\n%s"
        "%s"
        ),
      *CostText,
      *RequirementsText
    ));
  }
  else
  {
    UpgradeDetails = FText::FromString(FString::Printf(TEXT(
        "\n\n%s"
        ),
      *CostText
    ));
  }
}

FString UUpgradeManager::GetUpgradeDescription(
  const FUpgradeInfoParams& Params,
  int32 Level,
  bool bNextLevel
  )
{
  const FAuraUpgradeInfo& Info = GetUpgradeInfo(Params);

  if (!Info.Ability) return FString();

  FText Name;
  FText Description;
  FText Details;
  GetUpgradeFormattedTexts(Info, Level, bNextLevel, Name, Description, Details);

  return Name.ToString() + Description.ToString() + Details.ToString();
}

bool UUpgradeManager::HasResourcesToUnlock(const FGameplayTag& UpgradeTag, int32 Level)
{
  if (!GetAuraPlayerState()) return false;

  const FAuraUpgradeInfo& Info = UpgradeInfo->FindUpgradeInfoByTag(UpgradeTag);

  TMap<FGameplayTag, int32> CostMap;
  for (const auto& [Resource, ScalableCost] : Info.Cost)
  {
    CostMap.Add(Resource, ScalableCost.AsInteger(Level));
  }

  return AuraPlayerState.Get()->CanAffordResourceCost(CostMap);
}

bool UUpgradeManager::HasRequiredUpgrades(const FGameplayTag& UpgradeTag)
{
  if (!GetAuraPlayerState()) return false;

  const FAuraUpgradeInfo& Info = UpgradeInfo->FindUpgradeInfoByTag(UpgradeTag);

  const UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (!AuraASC) return false;
  
  TArray<FGameplayAbilitySpecHandle> AbilitiesHandles;
  AuraASC->FindAllAbilitiesWithTags(AbilitiesHandles, Info.Requirements);

  return AbilitiesHandles.Num() == Info.Requirements.Num();
}

void UUpgradeManager::UnlockUpgrade(const FGameplayTag& UpgradeTag)
{
  const FAuraUpgradeInfo& Info = UpgradeInfo->FindUpgradeInfoByTag(UpgradeTag);
  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (FGameplayAbilitySpec* AbilitySpec = AuraASC->GetSpecFromAbilityTag(UpgradeTag))
  {
    AbilitySpec->Level += 1;
    AuraASC->MarkAbilitySpecDirty(*AbilitySpec);
  }
  else
  {
    GiveUpgrade(Info, AuraASC);
  }
}

AAuraPlayerState* UUpgradeManager::GetAuraPlayerState()
{
  if (AuraPlayerState == nullptr)
  {
    AuraPlayerState = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
  }

  return AuraPlayerState.Get();
}

void UUpgradeManager::GiveUpgrade(
  const FAuraUpgradeInfo& AuraUpgradeInfo,
  UAuraAbilitySystemComponent* AuraASC
  )
{
  if (AuraUpgradeInfo.Ability)
  {
    FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AuraUpgradeInfo.Ability, 1);
    AuraASC->UnlockAbility(AbilitySpec);
  }

  if (AuraUpgradeInfo.GameplayEffect)
  {
    const FGameplayEffectContextHandle ContextHandle = AuraASC->MakeEffectContext();
    AuraASC->ApplyGameplayEffectToSelf(AuraUpgradeInfo.GameplayEffect.GetDefaultObject(), 1, ContextHandle);
  }
}
