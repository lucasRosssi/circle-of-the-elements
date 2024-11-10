// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"

#include "UpgradeMenuWidgetController.generated.h"

struct FAuraUpgradeInfo;
enum class ECharacterName : uint8;
class UUpgradeManager;
struct FUpgradeInfoParams;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
  FOnUpgradeGlobeSelected,
  const FAuraUpgradeInfo&,
  UpgradeInfo,
  int32,
  Level,
  bool,
  bCanUnlock
);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UUpgradeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
  virtual void BroadcastInitialValues() override;
  virtual void BindCallbacksToDependencies() override;

  UFUNCTION(BlueprintCallable)
  void UpgradeGlobeSelected(const FAuraUpgradeInfo& AuraUpgradeInfo);

  UFUNCTION(BlueprintCallable)
  void UnlockUpgrade(const FGameplayTag& UpgradeTag);

  UFUNCTION(BlueprintCallable)
  void EquipUpgrade(const FGameplayTag& UpgradeTag);
  UFUNCTION(BlueprintCallable)
  void UnequipUpgrade(const FGameplayTag& UpgradeTag);

  UFUNCTION(BlueprintCallable)
  FString GetUpgradeDescription(const FAuraUpgradeInfo& AuraUpgradeInfo, int32 Level);

  UFUNCTION(BlueprintPure)
  bool CanBeUnlocked(const FGameplayTag& UpgradeTag);

  UPROPERTY(BlueprintAssignable)
  FOnUpgradeGlobeSelected OnUpgradeGlobeSelectedDelegate;

protected:
  UFUNCTION(BlueprintPure)
  UUpgradeManager* GetUpgradeManager();

private:
  UPROPERTY()
  TObjectPtr<UUpgradeManager> UpgradeManager;
};
