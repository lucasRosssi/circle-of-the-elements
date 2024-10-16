// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class UUpgradeMenuWidgetController;
class USkillMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAuraUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
		const FWidgetControllerParams& WCParams
	);
	USkillMenuWidgetController* GetSkillMenuWidgetController(
		const FWidgetControllerParams& WCParams
	);
  UUpgradeMenuWidgetController* GetUpgradeMenuWidgetController(
    const FWidgetControllerParams& WCParams
  );

	UFUNCTION(BlueprintPure)
	UAuraUserWidget* GetOverlayWidget() const { return OverlayWidget; }

	void InitOverlay(
		APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASC,
		UAttributeSet* AS);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Root Widget")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Widget Controller")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY(EditDefaultsOnly, Category="Widget Controller")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY(EditDefaultsOnly, Category="Widget Controller")
	TSubclassOf<USkillMenuWidgetController> SkillMenuWidgetControllerClass;
  UPROPERTY(EditDefaultsOnly, Category="Widget Controller")
  TSubclassOf<UUpgradeMenuWidgetController> UpgradeMenuWidgetControllerClass;

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY()
	TObjectPtr<USkillMenuWidgetController> SkillMenuWidgetController;
  UPROPERTY()
  TObjectPtr<UUpgradeMenuWidgetController> UpgradeMenuWidgetController;
};
