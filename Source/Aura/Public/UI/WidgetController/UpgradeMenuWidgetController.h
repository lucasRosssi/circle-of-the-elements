// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"

#include "UpgradeMenuWidgetController.generated.h"

enum class ECharacterName : uint8;
class UUpgradeManager;
struct FUpgradeInfoParams;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
  FOnUpgradeGlobeSelected,
  bool,
  bCanUnlock,
  FString,
  DescriptionString
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
	void OnUpgradeGlobeSelected(const FGameplayTag& UpgradeTag, ECharacterName CharacterName);

	UFUNCTION(BlueprintCallable)
	void UnlockUpgrade(const FGameplayTag& UpgradeTag);

	UFUNCTION(BlueprintCallable)
	FString GetUpgradeDescription(const FUpgradeInfoParams& Params, int32 Level);

  UPROPERTY(BlueprintAssignable)
  FOnUpgradeGlobeSelected OnUpgradeGlobeSelectedDelegate;

protected:
  UFUNCTION(BlueprintPure)
  UUpgradeManager* GetUpgradeManager();

private:
  UPROPERTY()
  TObjectPtr<UUpgradeManager> UpgradeManager;
};
