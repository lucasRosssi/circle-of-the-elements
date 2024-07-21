// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/Public/CommonActivatableWidget.h"

#include "AuraUserWidget.generated.h"

class AAuraPlayerState;
class AMainPlayerController;
class AAuraHero;
class UCommonActivatableWidgetStack;
/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// Common UI overrides
	// End Common UI overrides
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraHero* GetOwningHero();

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AMainPlayerController* GetOwningMainPlayerController();

	UFUNCTION(BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraPlayerState* GetOwningAuraPlayerState();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UPROPERTY(BlueprintReadWrite)
	UCommonActivatableWidgetStack* OwningStack;

private:
	UPROPERTY()
	AAuraHero* Hero = nullptr;
	UPROPERTY()
	AMainPlayerController* MainPlayerController = nullptr;
	UPROPERTY()
	AAuraPlayerState* AuraPlayerState = nullptr;
};
