// Copyright Lucas Rossi


#include "Actor/Level/Interactable.h"

#include "Components/InteractComponent.h"

AInteractable::AInteractable()
{
  PrimaryActorTick.bCanEverTick = false;

  StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
  SetRootComponent(StaticMesh);

  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetupInteractAreaAttachment(GetRootComponent());
  InteractComponent->EnableInteraction();
  InteractComponent->bDisableAfterInteraction = false;
}


UInteractComponent* AInteractable::GetInteractComponent_Implementation() const
{
  return InteractComponent;
}

void AInteractable::BeginPlay()
{
  Super::BeginPlay();

  if (!bActive)
  {
    InteractComponent->DisableInteraction();
  }
}


