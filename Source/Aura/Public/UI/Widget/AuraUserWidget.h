// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

class AAuraPlayerState;
class AMainPlayerController;
class AAuraCharacter;
/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	AAuraCharacter* GetOwningAuraCharacter();

	UFUNCTION(BlueprintCallable)
	AMainPlayerController* GetOwningMainPlayerController();

	UFUNCTION(BlueprintCallable)
	AAuraPlayerState* GetOwningAuraPlayerState();
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
