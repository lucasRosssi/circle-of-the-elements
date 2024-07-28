// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UINavigation/Public/UINavWidget.h"
#include "AuraUserWidget.generated.h"

class UInputAction;
class AAuraPlayerState;
class AAuraPlayerController;
class AAuraHero;

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUINavWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraHero* GetOwningHero();

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraPlayerController* GetOwningAuraPlayerController();

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraPlayerState* GetOwningAuraPlayerState();

	UFUNCTION(BlueprintPure)
	UTexture2D* GetInputActionIcon(const UInputAction* Action);

	UFUNCTION(BlueprintImplementableEvent, Category="Input")
	void OnChangeInputDevice(EInputType InputType);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

private:
	UPROPERTY()
	AAuraHero* Hero = nullptr;
	UPROPERTY()
	AAuraPlayerController* MainPlayerController = nullptr;
	UPROPERTY()
	AAuraPlayerState* AuraPlayerState = nullptr;
};
