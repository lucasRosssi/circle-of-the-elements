// Copyright Lucas Rossi


#include "Interfaces/InteractInterface.h"

bool IInteractInterface::Implements(const UObject* Object)
{
	return IsValid(Object) && Object->Implements<UInteractInterface>();
}

void IInteractInterface::Safe_Interact(UObject* Object, const AController* Controller)
{
  if (!Implements(Object)) return;

  Execute_Interact(Object, Controller);
}

UInteractComponent* IInteractInterface::Safe_GetInteractComponent(const UObject* Object)
{
  if (!Implements(Object)) return nullptr;

  return Execute_GetInteractComponent(Object);
}

FGameplayEventData IInteractInterface::Safe_GetAbilityEventData(const UObject* Object)
{
  if (!Implements(Object)) return FGameplayEventData();

  return Execute_GetAbilityEventData(Object);
}
