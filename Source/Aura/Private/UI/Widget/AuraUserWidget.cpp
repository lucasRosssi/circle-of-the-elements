// Copyright Lucas Rossi


#include "UI/Widget/AuraUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "Player/MainPlayerController.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();

	// Propagate widget controller to children
	WidgetTree->ForEachWidget(
		[&](UWidget* Widget)
		{
			if (UAuraUserWidget* AuraWidget = Cast<UAuraUserWidget>(Widget))
			{
					AuraWidget->SetWidgetController(InWidgetController);
			}
		}
	);
}

AAuraCharacter* UAuraUserWidget::GetOwningAuraCharacter()
{
	if (AuraCharacter == nullptr)
	{
		AuraCharacter = Cast<AAuraCharacter>(GetOwningPlayerPawn());
	}

	return AuraCharacter;
}

AMainPlayerController* UAuraUserWidget::GetOwningMainPlayerController()
{
	if (MainPlayerController == nullptr)
	{
		MainPlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
	}

	return MainPlayerController;
}

AAuraPlayerState* UAuraUserWidget::GetOwningAuraPlayerState()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(GetOwningPlayerState());
	}

	return AuraPlayerState;
}
