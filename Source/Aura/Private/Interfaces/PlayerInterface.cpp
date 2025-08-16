// Copyright Lucas Rossi


#include "Interfaces/PlayerInterface.h"

bool IPlayerInterface::Implements(const UObject* Object)
{
	return Object != nullptr && Object->Implements<UPlayerInterface>();
}

void IPlayerInterface::SafeExec_AddToXP(UObject* Object, int32 InXP)
{
	if (!Implements(Object)) return;

	Execute_AddToXP(Object, InXP);
}

void IPlayerInterface::Safe_SetInteractMessageVisible(UObject* Object, const FText& InteractText)
{
  if (!Implements(Object)) return;

  Execute_SetInteractMessageVisible(Object, InteractText);
}

void IPlayerInterface::Safe_AddInteractableToList(UObject* Object, UInteractComponent* InteractableComponent)
{
  if (!Implements(Object)) return;

  Execute_AddInteractableToList(Object, InteractableComponent);
}

void IPlayerInterface::Safe_RemoveInteractableFromList(UObject* Object, UInteractComponent* InteractableComponent)
{
  if (!Implements(Object)) return;

  Execute_RemoveInteractableFromList(Object, InteractableComponent);
}
