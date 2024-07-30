// Copyright Lucas Rossi


#include "Interaction/PlayerInterface.h"

bool IPlayerInterface::Implements(const UObject* Object)
{
	return Object != nullptr && Object->Implements<UPlayerInterface>();
}

void IPlayerInterface::SafeExec_AddToXP(UObject* Object, int32 InXP)
{
	if (!Implements(Object)) return;

	Execute_AddToXP(Object, InXP);
}
