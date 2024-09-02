// Copyright Lucas Rossi


#include "Game/Components/UIManager.h"

#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"

UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

UAuraUserWidget* UUIManager::BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	return CreateWidget<UAuraUserWidget>(AuraPlayerController, WidgetClass);
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
