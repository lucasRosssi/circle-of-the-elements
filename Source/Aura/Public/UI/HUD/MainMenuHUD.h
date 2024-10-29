// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMainMenuWidgetController;
class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class AURA_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
  UMainMenuWidget* GetMainMenuWidget() const { return MainMenuWidget; }
  UMainMenuWidgetController* GetMainMenuWidgetController();
  
protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UMainMenuWidgetController> MainMenuWidgetControllerClass;
private:
  UPROPERTY()
  TObjectPtr<UMainMenuWidget> MainMenuWidget;
  UPROPERTY()
  TObjectPtr<UMainMenuWidgetController> MainMenuWidgetController;
};
