// Copyright Lucas Rossi


#include "Actor/Level/Interactable.h"

#include "Components/InteractComponent.h"
#include "Components/SphereComponent.h"

AInteractable::AInteractable()
{
  PrimaryActorTick.bCanEverTick = false;

  StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
  SetRootComponent(StaticMesh);
  InteractSphere = CreateDefaultSubobject<USphereComponent>("InteractSphere");
  InteractSphere->SetupAttachment(GetRootComponent());
  InteractSphere->SetSphereRadius(100.f);

  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetInteractAreaComponent(InteractSphere);
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


