// Copyright Lucas Rossi


#include "Managers/UpgradeManager.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/UpgradeAbility.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "AbilitySystem/GameplayEffects/UpgradeEffect.h"

void UUpgradeManager::GiveAcquiredUpgrades(AActor* Actor)
{
  if (AcquiredUpgrades.Num() > 0)
  {
    for (auto UpgradeTagIterator = AcquiredUpgrades.CreateConstIterator(); UpgradeTagIterator; ++UpgradeTagIterator)
    {
      const FAuraUpgradeInfo& Info = UpgradeInfo->FindUpgradeInfoByTag(*UpgradeTagIterator);
      UAuraAbilitySystemComponent* AuraASC = UAuraAbilitySystemLibrary::GetAuraAbilitySystemComponent(Actor);
      GiveUpgrade(Info, AuraASC);
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
  FText& UpgradeName,
  FText& UpgradeDescription,
  FText& UpgradeDetails
)
{
  UpgradeName = FText::FromString(FString::Printf(
    TEXT("<Title>%s</>"),
    *AuraUpgradeInfo.Name.ToString()
    ));

  UpgradeDescription = AuraUpgradeInfo.Description;
  const UUpgradeAbility* Ability = AuraUpgradeInfo.Ability.GetDefaultObject();
  UAuraAbilitySystemLibrary::FormatAbilityDescriptionAtLevel(
    Ability,
    1,
    UpgradeDescription
    );

  FString CostText;
  FString RequirementsText;
  UAuraAbilitySystemLibrary::MakeUpgradeDetailsText(
    Ability,
    1,
    CostText,
    RequirementsText
    );
  UpgradeDetails = FText::FromString(FString::Printf(TEXT(
      "%s"
      "%s"
      ),
    *CostText,
    *RequirementsText
  ));
}

void UUpgradeManager::GiveUpgrade(const FAuraUpgradeInfo& AuraUpgradeInfo, UAuraAbilitySystemComponent* AuraASC)
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
