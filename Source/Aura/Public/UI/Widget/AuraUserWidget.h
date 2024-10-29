// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "Interfaces/WidgetControllerInterface.h"
#include "AuraUserWidget.generated.h"

enum class EMenuBlurMode : uint8;
class UUIManager;
class UInputAction;
class AAuraPlayerState;
class AAuraPlayerController;
class AAuraHero;

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UBaseWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

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
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Focus")
	EMenuBlurMode BlurMode;

protected:
	UFUNCTION(BlueprintPure)
	UUIManager* GetUIManager();

private:
	UPROPERTY()
	AAuraHero* Hero = nullptr;
	UPROPERTY()
	AAuraPlayerController* AuraPlayerController = nullptr;
	UPROPERTY()
	AAuraPlayerState* AuraPlayerState = nullptr;
};
