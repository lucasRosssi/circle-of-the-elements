// Copyright Lucas Rossi


#include "Managers/UIManager.h"

#include "UI/Widget/AuraUserWidget.h"
#include "GameFramework/PlayerController.h"

UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

UAuraUserWidget* UUIManager::BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	return CreateWidget<UAuraUserWidget>(PlayerController, WidgetClass);
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
