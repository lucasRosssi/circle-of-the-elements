// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SkillMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FSkillGlobeSelected,
	bool,
	bCanSpendPoint,
	bool,
	bCanEquip,
	FString,
	DescriptionString
);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USkillMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FSkillGlobeSelected SkillGlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable)
	void SkillGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPoint(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable)
	FString GetSkillDescription(FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable)
	void EquipToInputPressed(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(BlueprintPure)
	TArray<FAuraAbilityInfo> GetAcquiredAbilities() const { return AcquiredAbilityInfos; }

private:
	static void ShouldEnableInteractions(
		const FGameplayTag& AbilityStatus,
		int32 SkillPoints,
		bool& bShouldEnableSpendPoint,
		bool& bShouldEnableEquip
	);

	UFUNCTION()
	void OnAbilitySelected(const FAuraAbilityInfo& AuraAbilityInfo);
	
	TArray<FAuraAbilityInfo> AcquiredAbilityInfos;
};
