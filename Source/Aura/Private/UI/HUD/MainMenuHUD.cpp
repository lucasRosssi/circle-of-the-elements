// Copyright Lucas Rossi


#include "UI/HUD/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/MainMenuWidget.h"
#include "UI/WidgetController/MainMenuWidgetController.h"

UMainMenuWidgetController* AMainMenuHUD::GetMainMenuWidgetController()
{
  if (MainMenuWidgetController == nullptr)
  {
    MainMenuWidgetController = NewObject<UMainMenuWidgetController>(
      this,
      MainMenuWidgetControllerClass
      );
  }

  return MainMenuWidgetController;
}

void AMainMenuHUD::BeginPlay()
{
  Super::BeginPlay();

  GetMainMenuWidgetController()->InitializeSaveSlots();

  MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
  MainMenuWidget->SetWidgetController(GetMainMenuWidgetController());
  MainMenuWidget->AddToViewport();
}
