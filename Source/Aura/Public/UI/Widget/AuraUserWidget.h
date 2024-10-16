// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/WidgetControllerInterface.h"
#include "UINavigation/Public/UINavWidget.h"
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
class AURA_API UAuraUserWidget : public UUINavWidget, public IWidgetControllerInterface
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

  // Widget Controller Interface
  virtual void AssignWidgetController_Implementation(UObject* InWidgetController) override;
  // END Widget Controller Interface
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Focus")
	EMenuBlurMode BlurMode;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

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
