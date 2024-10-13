// Copyright Lucas Rossi


#include "UI/WidgetController/UpgradeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "Managers/UpgradeManager.h"
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

void UUpgradeMenuWidgetController::OnUpgradeGlobeSelected(
  const FGameplayTag& UpgradeTag,
  ECharacterName CharacterName
  )
{
  const bool bCanUnlock = GetUpgradeManager()->HasResourcesToUnlock(UpgradeTag) &&
                          UpgradeManager->HasRequiredUpgrades(UpgradeTag);

  UAuraAbilitySystemComponent* AuraASC = GetAuraAbilitySystemComponent();
  const int32 Level = AuraASC->GetAbilityLevelFromTag(UpgradeTag);
  
  FUpgradeInfoParams Params;
  Params.UpgradeTag = UpgradeTag;
  Params.ElementTag = UpgradeTag.RequestDirectParent();
  Params.CharacterName = CharacterName;
  
	const FString Description = GetUpgradeDescription(Params, Level > 1 ? Level : 1);
  
	OnUpgradeGlobeSelectedDelegate.Broadcast(bCanUnlock, Description);
}

void UUpgradeMenuWidgetController::UnlockUpgrade(
	const FGameplayTag& UpgradeTag
	)
{
	if (!GetUpgradeManager()) return;

  UpgradeManager->UnlockUpgrade(UpgradeTag);
}

FString UUpgradeMenuWidgetController::GetUpgradeDescription(const FUpgradeInfoParams& Params, int32 Level)
{
  const bool bNextLevel = Level > 1 && GetUpgradeManager()->HasResourcesToUnlock(Params.UpgradeTag, Level);
  
	return UpgradeManager->GetUpgradeDescription(Params, Level, bNextLevel);
}

UUpgradeManager* UUpgradeMenuWidgetController::GetUpgradeManager()
{
  if (UpgradeManager == nullptr)
  {
    UpgradeManager = UAuraSystemsLibrary::GetUpgradeManager(this);
  }

  return UpgradeManager;
}
