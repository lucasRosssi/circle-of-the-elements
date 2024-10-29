// Copyright Lucas Rossi


#include "UI/Widget/AuraUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Character/AuraHero.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"

void UAuraUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningAuraPlayerController())
	{
		GetOwningAuraPlayerController()->GetUINavComponent()
			->InputTypeChangedDelegate.AddUniqueDynamic(
				this,
				&UAuraUserWidget::OnChangeInputDevice
				);
	}
}

AAuraHero* UAuraUserWidget::GetOwningHero()
{
	if (Hero == nullptr)
	{
		Hero = Cast<AAuraHero>(GetOwningPlayerPawn());
	}

	return Hero;
}

AAuraPlayerController* UAuraUserWidget::GetOwningAuraPlayerController()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(GetOwningPlayer());
	}

	return AuraPlayerController;
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
	if (!GetOwningAuraPlayerController()) return nullptr;
	
	const UUINavPCComponent* UINavComponent = GetOwningAuraPlayerController()->GetUINavComponent();
	
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

UUIManager* UAuraUserWidget::GetUIManager()
{
	return GetOwningAuraPlayerController()->GetUIManager();
}
