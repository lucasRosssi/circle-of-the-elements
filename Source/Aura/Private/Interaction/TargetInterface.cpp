// Copyright Lucas Rossi


#include "Interaction/TargetInterface.h"

bool ITargetInterface::Implements(const UObject* Object)
{
	return Object != nullptr && Object->Implements<UTargetInterface>();
}

void ITargetInterface::SafeExec_HighlightActor(UObject* Object, AActor* InstigatorActor)
{
	if (!Implements(Object)) return;

	Execute_HighlightActor(Object, InstigatorActor);
}

void ITargetInterface::SafeExec_UnHighlightActor(UObject* Object)
{
	if (!Implements(Object)) return;

	Execute_UnHighlightActor(Object);
}
