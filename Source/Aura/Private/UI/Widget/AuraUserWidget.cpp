// Copyright Lucas Rossi


#include "UI/Widget/AuraUserWidget.h"

#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "Player/MainPlayerController.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

AAuraCharacter* UAuraUserWidget::GetOwningAuraCharacter()
{
	AAuraCharacter* OwningCharacter = Cast<AAuraCharacter>(GetOwningPlayerPawn());

	return OwningCharacter;
}

AMainPlayerController* UAuraUserWidget::GetOwningMainPlayerController()
{
	AMainPlayerController* Controller = Cast<AMainPlayerController>(GetOwningPlayer());

	return Controller;
}

AAuraPlayerState* UAuraUserWidget::GetOwningAuraPlayerState()
{
	AAuraPlayerState* PlayerState = Cast<AAuraPlayerState>(GetOwningPlayerState());

	return PlayerState;
}
