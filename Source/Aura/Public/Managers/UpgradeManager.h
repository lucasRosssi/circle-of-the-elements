// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "AbilitySystem/Data/UpgradeInfo.h"
#include "UpgradeManager.generated.h"

class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UUpgradeInfo;
struct FUpgradeInfoParams;
struct FAuraUpgradeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
  FOnUpgradeUnlock,
  const FAuraUpgradeInfo&, AuraUpgradeInfo,
  int32, Level
  );

/**
 * 
 */
UCLASS()
class AURA_API UUpgradeManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
  void GiveAcquiredUpgrades();

  UFUNCTION(BlueprintPure, Category="Manager|Upgrade")
	TMap<FGameplayTag, FAuraUpgradeInfo> GetElementUpgrades(
		ECharacterName CharacterName,
		const FGameplayTag ElementTag
		);
	UFUNCTION(BlueprintPure, Category="Manager|Upgrade")
	FAuraUpgradeInfo GetAuraUpgradeInfo(const FUpgradeInfoParams& Params);

	UFUNCTION(BlueprintCallable, Category="Manager|Upgrade|Description")
	void GetUpgradeFormattedTexts(
		const FAuraUpgradeInfo& AuraUpgradeInfo,
		int32 Level,
		bool bNextLevel,
		FText& UpgradeName,
		FText& UpgradeDescription,
		FText& UpgradeDetails
		);
  UFUNCTION(BlueprintPure, Category="Manager|Upgrade|Description")
  FString GetUpgradeDescription(
    const FAuraUpgradeInfo& AuraUpgradeInfo,
    int32 Level,
    bool bNextLevel
  );

  bool HasResourcesToUnlock(const FGameplayTag& UpgradeTag);
  bool HasRequiredUpgrades(const FGameplayTag& UpgradeTag);
  bool IsMaxed(const FGameplayTag& UpgradeTag);
  bool IsMaxed(const FAuraUpgradeInfo& AuraUpgradeInfo);

  void UnlockUpgrade(const FGameplayTag& UpgradeTag, int32 Level = 1);
  void EquipUpgrade(const FGameplayTag& UpgradeTag, bool bSkipMutuallyExclusiveCheck = false);
  void UnequipUpgrade(const FGameplayTag& UpgradeTag);

  UPROPERTY(BlueprintAssignable, Category="Manager|Upgrade")
  FOnUpgradeUnlock OnUpgradeUnlockDelegate;

protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Upgrade",
		meta=(Categories="Upgrades")
		)
	TMap<FGameplayTag, int32> AcquiredUpgrades;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Manager|Upgrade",
    meta=(Categories="Upgrades")
    )
  FGameplayTagContainer EquippedUpgrades;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Upgrade",
		meta=(Categories="Upgrades")
		)
	FGameplayTagContainer BlockedUpgrades;

private:
  AAuraPlayerState* GetAuraPlayerState();
  void GiveUpgrade(const FAuraUpgradeInfo& AuraUpgradeInfo, UAuraAbilitySystemComponent* AuraASC, int32 Level);

  UUpgradeInfo* GetUpgradeInfo();

  TWeakObjectPtr<AAuraPlayerState> AuraPlayerState;

  UPROPERTY()
  TObjectPtr<UUpgradeInfo> UpgradeInfo;
};
