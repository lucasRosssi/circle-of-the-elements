// Copyright Lucas Rossi


#include "UI/Widget/AuraUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Character/AuraHero.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"

void UAuraUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningMainPlayerController())
	{
		GetOwningMainPlayerController()->GetUINavComponent()->InputTypeChangedDelegate.AddDynamic(
			this,
			&UAuraUserWidget::OnChangeInputDevice
		);
	}
}

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

AAuraHero* UAuraUserWidget::GetOwningHero()
{
	if (Hero == nullptr)
	{
		Hero = Cast<AAuraHero>(GetOwningPlayerPawn());
	}

	return Hero;
}

AAuraPlayerController* UAuraUserWidget::GetOwningMainPlayerController()
{
	if (MainPlayerController == nullptr)
	{
		MainPlayerController = Cast<AAuraPlayerController>(GetOwningPlayer());
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

UTexture2D* UAuraUserWidget::GetInputActionIcon(const UInputAction* Action)
{
	if (!GetOwningMainPlayerController()) return nullptr;
	
	const UUINavPCComponent* UINavComponent = GetOwningMainPlayerController()->GetUINavComponent();
	
	EInputRestriction InputRestriction;
	if (UINavComponent->IsUsingGamepad()) InputRestriction = EInputRestriction::Gamepad;
	else InputRestriction = EInputRestriction::Keyboard_Mouse;
	
	return UINavComponent->GetEnhancedInputIcon(
		Action,
		EInputAxis::X,
		EAxisType::None,
		InputRestriction
		);
}
