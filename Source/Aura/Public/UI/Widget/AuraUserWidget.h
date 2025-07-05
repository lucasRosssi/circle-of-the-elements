// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseWidget.h"
#include "GameplayTagContainer.h"
#include "Interfaces/WidgetControllerInterface.h"
#include "AuraUserWidget.generated.h"

struct FAuraMenuInput;
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

  UFUNCTION(BlueprintPure, Category="Input")
  TArray<FAuraMenuInput> GetMenuInputs();
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Menu Info")
	EMenuBlurMode BlurMode;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Menu Info", meta=(Categories="Menu"))
  FGameplayTag MenuTag = FGameplayTag();
  
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
