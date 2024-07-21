// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/Public/CommonActivatableWidget.h"

#include "AuraUserWidget.generated.h"

class AAuraPlayerState;
class AMainPlayerController;
class AAuraCharacter;
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
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraCharacter* GetOwningAuraCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AMainPlayerController* GetOwningMainPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (HidePin = "Target", DefaultToSelf = "Target"))
	AAuraPlayerState* GetOwningAuraPlayerState();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

private:
	UPROPERTY()
	AAuraCharacter* AuraCharacter = nullptr;
	UPROPERTY()
	AMainPlayerController* MainPlayerController = nullptr;
	UPROPERTY()
	AAuraPlayerState* AuraPlayerState = nullptr;
};
