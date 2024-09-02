// Copyright Lucas Rossi


#include "Game/Components/UIManager.h"

#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"

UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

UAuraUserWidget* UUIManager::GetOverlayWidget()
{
	if (OverlayWidget == nullptr)
	{
		OverlayWidget = GetAuraPlayerController()->GetAuraHUD()->GetOverlayWidget();
	}

	return OverlayWidget;
}

UAuraUserWidget* UUIManager::BuildWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	return CreateWidget<UAuraUserWidget>(AuraPlayerController, WidgetClass);
}

void UUIManager::OpenWidget(UAuraUserWidget* Widget)
{
	OverlayWidget->GoToBuiltWidget(Widget, false);
	OnNewMenuOpenedDelegate.Broadcast(Widget);
}

UAuraUserWidget* UUIManager::BuildAndOpenWidget(TSubclassOf<UAuraUserWidget> WidgetClass)
{
	UAuraUserWidget* Widget = BuildWidget(WidgetClass);
	OpenWidget(Widget);

	return Widget;
}
