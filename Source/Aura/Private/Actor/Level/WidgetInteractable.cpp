// Copyright Lucas Rossi


#include "Actor/Level/WidgetInteractable.h"

#include "Managers/UIManager.h"
#include "Player/AuraPlayerController.h"

void AWidgetInteractable::Interact_Implementation(const AController* Controller)
{
  const AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(Controller);
  if (!AuraPC) return;
  
  AuraPC->GetUIManager()->BuildAndOpenWidget(WidgetClass);
}
