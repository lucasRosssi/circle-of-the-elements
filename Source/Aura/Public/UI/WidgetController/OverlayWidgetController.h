// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class ABossEnemy;
struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionChange, bool, bStarting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowBossHealthBar, ABossEnemy*, Boss);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideBossHealthBar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowControls, UAuraUserWidget*, AuraWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideControls);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Location")
	FTransitionChange StartTransitionDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Location")
	FShowBossHealthBar ShowBossHealthBarDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Location")
	FHideBossHealthBar HideBossHealthBarDelegate;

  UPROPERTY(BlueprintAssignable, Category="Input")
  FShowControls ShowControlsDelegate;
  UPROPERTY(BlueprintAssignable, Category="Input")
  FHideControls HideControlsDelegate;
	
	UFUNCTION(BlueprintCallable)
	void StartTransition();
	UFUNCTION(BlueprintCallable)
	void EndTransition();

	UFUNCTION(BlueprintCallable)
	void ShowBossHealthBar(ABossEnemy* Boss);
	UFUNCTION(BlueprintCallable)
	void HideBossHealthBar();

  UFUNCTION(BlueprintCallable)
  void ShowControls(UAuraUserWidget* AuraWidget);
  UFUNCTION(BlueprintCallable)
  void HideControls();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnAbilityEquipped(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		const FGameplayTag& InputTag,
		const FGameplayTag& PreviousInputTag
		);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAbilityCharges(const FGameplayTag& ChargeTag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
