// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UINavigation/Public/UINavWidget.h"
#include "AuraUserWidget.generated.h"

class AAuraPlayerState;
class AMainPlayerController;
class AAuraHero;
/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUINavWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraHero* GetOwningHero();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AMainPlayerController* GetOwningMainPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraPlayerState* GetOwningAuraPlayerState();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

private:
	UPROPERTY()
	AAuraHero* Hero = nullptr;
	UPROPERTY()
	AMainPlayerController* MainPlayerController = nullptr;
	UPROPERTY()
	AAuraPlayerState* AuraPlayerState = nullptr;
};
