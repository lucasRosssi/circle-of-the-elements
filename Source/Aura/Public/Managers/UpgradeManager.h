// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "UpgradeManager.generated.h"

class UAuraAbilitySystemComponent;
class UUpgradeInfo;
struct FUpgradeInfoParams;
struct FAuraUpgradeInfo;
/**
 * 
 */
UCLASS()
class AURA_API UUpgradeManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
  void GiveAcquiredUpgrades(AActor* Actor);

  UFUNCTION(BlueprintPure, Category="Manager|Upgrade")
	TMap<FGameplayTag, FAuraUpgradeInfo> GetElementUpgrades(
		ECharacterName CharacterName,
		const FGameplayTag ElementTag
		);
	UFUNCTION(BlueprintPure, Category="Manager|Upgrade")
	FAuraUpgradeInfo GetUpgradeInfo(const FUpgradeInfoParams& Params);

	UFUNCTION(BlueprintCallable, Category="Manager|Upgrade|Description")
	void GetUpgradeFormattedTexts(
		const FAuraUpgradeInfo& AuraUpgradeInfo,
		FText& UpgradeName,
		FText& UpgradeDescription,
		FText& UpgradeDetails
		);

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Game|Info")
  TObjectPtr<UUpgradeInfo> UpgradeInfo;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Upgrade",
		meta=(Categories="Upgrades")
		)
	FGameplayTagContainer AcquiredUpgrades;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Upgrade",
		meta=(Categories="Upgrades")
		)
	FGameplayTagContainer BlockedUpgrades;

private:
  void GiveUpgrade(const FAuraUpgradeInfo& AuraUpgradeInfo, UAuraAbilitySystemComponent* AuraASC);
};
