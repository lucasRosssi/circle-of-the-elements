// Copyright Lucas Rossi


#include "UI/WidgetController/UpgradeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "Managers/UpgradeManager.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void UUpgradeMenuWidgetController::BroadcastInitialValues()
{
  BroadcastAbilityInfo();
}

void UUpgradeMenuWidgetController::BindCallbacksToDependencies()
{
  GetAuraAbilitySystemComponent()->AbilityStateChanged.AddLambda(
    [this](
    const FGameplayTag& AbilityTag,
    const FGameplayTag& StatusTag,
    const FGameplayTag& InputTag,
    int32 NewLevel
  )
    {
      if (AbilityInfo)
      {
        FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
        Info.StatusTag = StatusTag;
        Info.InputTag = InputTag;
        AbilityInfoDelegate.Broadcast(Info);
      }
    }
  );
}

void UUpgradeMenuWidgetController::UpgradeGlobeSelected(const FAuraUpgradeInfo& AuraUpgradeInfo)
{
  if (!AuraUpgradeInfo.IsValid()) return;
  
  const bool bCanUnlock = GetUpgradeManager()->HasResourcesToUnlock(AuraUpgradeInfo.UpgradeTag) &&
    UpgradeManager->HasRequiredUpgrades(AuraUpgradeInfo.UpgradeTag);

  UAuraAbilitySystemComponent* AuraASC = GetAuraPlayerState()->GetAuraASC();

  if (!IsValid(AuraASC)) return;

  const int32 Level = AuraASC->GetAbilityLevelFromTag(AuraUpgradeInfo.AbilityTag);
  
  OnUpgradeGlobeSelectedDelegate.Broadcast(AuraUpgradeInfo, Level, bCanUnlock);
}

void UUpgradeMenuWidgetController::UnlockUpgrade(
  const FGameplayTag& UpgradeTag
)
{
  if (!GetUpgradeManager()) return;

  UpgradeManager->UnlockUpgrade(UpgradeTag);
}

FString UUpgradeMenuWidgetController::GetUpgradeDescription(const FAuraUpgradeInfo& AuraUpgradeInfo, int32 Level)
{
  const bool bNextLevel = Level > 0 && GetUpgradeManager()->HasResourcesToUnlock(AuraUpgradeInfo.UpgradeTag);

  return UpgradeManager->GetUpgradeDescription(AuraUpgradeInfo, Level, bNextLevel);
}

bool UUpgradeMenuWidgetController::CanBeUnlocked(const FGameplayTag& UpgradeTag)
{
  return GetUpgradeManager()->HasResourcesToUnlock(UpgradeTag) &&
    UpgradeManager->HasRequiredUpgrades(UpgradeTag) &&
    !UpgradeManager->IsMaxed(UpgradeTag);
}

UUpgradeManager* UUpgradeMenuWidgetController::GetUpgradeManager()
{
  if (UpgradeManager == nullptr)
  {
    UpgradeManager = UAuraSystemsLibrary::GetUpgradeManager(this);
  }

  return UpgradeManager;
}
