// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UINavWidget.h"
#include "Interfaces/WidgetControllerInterface.h"
#include "BaseWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBaseWidget : public UUINavWidget, public IWidgetControllerInterface
{
	GENERATED_BODY()

public:
  // Widget Controller Interface
  virtual void AssignWidgetController_Implementation(UObject* InWidgetController) override;
  // END Widget Controller Interface
	
  UFUNCTION(BlueprintCallable)
  void SetWidgetController(UObject* InWidgetController);
  
protected:
  UFUNCTION(BlueprintImplementableEvent)
  void WidgetControllerSet();
  
  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<UObject> WidgetController;

private:
  
};
