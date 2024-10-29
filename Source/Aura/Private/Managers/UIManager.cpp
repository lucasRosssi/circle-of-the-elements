// Copyright Lucas Rossi


#include "Managers/UIManager.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/HUD/AuraHUD.h"

void UUIManager::SetPlayerController(APlayerController* InPlayerController)
{
  PlayerController = InPlayerController;
}

void UUIManager::SetOverlayWidget(UAuraUserWidget* InWidget)
{
  OverlayWidget = InWidget;
}

void UUIManager::SetAuraHUD(AAuraHUD* InHUD)
{
  AuraHUD = InHUD;
}

UAuraUserWidget* UUIManager::BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	return CreateWidget<UAuraUserWidget>(PlayerController.Get(), WidgetClass);
}

UAuraUserWidget* UUIManager::OpenWidget(UAuraUserWidget* Widget)
{
	OverlayWidget->GoToBuiltWidget(Widget, false);
	OnNewMenuOpenedDelegate.Broadcast(Widget);

	return Widget;
}

UAuraUserWidget* UUIManager::BuildAndOpenWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	UAuraUserWidget* Widget = BuildWidget(WidgetClass);
	OpenWidget(Widget);

	return Widget;
}
