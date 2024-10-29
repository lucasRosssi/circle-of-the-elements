// Copyright Lucas Rossi


#include "Player/MainMenuController.h"

AMainMenuController::AMainMenuController()
{
  bShowMouseCursor = true;
}

void AMainMenuController::BeginPlay()
{
  Super::BeginPlay();

  FInputModeUIOnly InputMode = FInputModeUIOnly();
  InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  SetInputMode(InputMode);
}
