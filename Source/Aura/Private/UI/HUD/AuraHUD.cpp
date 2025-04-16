// Copyright Lucas Rossi


#include "UI/HUD/AuraHUD.h"

#include "Managers/UIManager.h"
#include "Player/AuraPlayerController.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/LoadoutWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SkillMenuWidgetController.h"
#include "UI/WidgetController/UpgradeMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(
			this,
			OverlayWidgetControllerClass
			);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this,
			AttributeMenuWidgetControllerClass
			);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

USkillMenuWidgetController* AAuraHUD::GetSkillMenuWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if (SkillMenuWidgetController == nullptr)
	{
		SkillMenuWidgetController = NewObject<USkillMenuWidgetController>(
			this,
			SkillMenuWidgetControllerClass
			);
		SkillMenuWidgetController->SetWidgetControllerParams(WCParams);
		SkillMenuWidgetController->BindCallbacksToDependencies();
	}

	return SkillMenuWidgetController;
}

UUpgradeMenuWidgetController* AAuraHUD::GetUpgradeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
  if (UpgradeMenuWidgetController == nullptr)
  {
    UpgradeMenuWidgetController = NewObject<UUpgradeMenuWidgetController>(
      this,
      UpgradeMenuWidgetControllerClass
      );
    UpgradeMenuWidgetController->SetWidgetControllerParams(WCParams);
    UpgradeMenuWidgetController->BindCallbacksToDependencies();
  }

  return UpgradeMenuWidgetController;
}

ULoadoutWidgetController* AAuraHUD::GetLoadoutWidgetController(const FWidgetControllerParams& WCParams)
{
  if (LoadoutWidgetController == nullptr)
  {
    LoadoutWidgetController = NewObject<ULoadoutWidgetController>(
      this,
      LoadoutWidgetControllerClass
      );
    LoadoutWidgetController->SetWidgetControllerParams(WCParams);
    LoadoutWidgetController->BindCallbacksToDependencies();
  }

  return LoadoutWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                           UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class unitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class unitialized, please fill out BP_AuraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();

	GetSkillMenuWidgetController(WidgetControllerParams);
	GetAttributeMenuWidgetController(WidgetControllerParams);
	
	if (const AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(PC))
	{
		AuraPC->GetUIManager()->SetOverlayWidget(OverlayWidget);
		AuraPC->GetUIManager()->SetAuraHUD(this);
	}
}
