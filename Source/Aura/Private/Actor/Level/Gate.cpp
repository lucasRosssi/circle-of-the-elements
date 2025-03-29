// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Components/InteractComponent.h"
#include "Managers/CombatManager.h"
#include "Utils/AuraSystemsLibrary.h"

AGate::AGate()
{
  GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
  SetRootComponent(GateMesh);

  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetupInteractAreaAttachment(GetRootComponent());
}

void AGate::Interact_Implementation(const AController* Controller)
{
  OnInteracted(Controller);
}

UInteractComponent* AGate::GetInteractComponent_Implementation() const
{
  return InteractComponent;
}

void AGate::DeactivateGate()
{
  bActive = false;
}

void AGate::BeginPlay()
{
  Super::BeginPlay();

  UAuraSystemsLibrary::GetOnCombatFinishedDelegate(this).AddDynamic(this, &AGate::OnCombatFinished);
}

void AGate::OnCombatFinished(FName InAreaName)
{
  if (InAreaName != AreaName) return;

  Enable();
}

void AGate::Enable()
{
  if (!bActive) return;

  InteractComponent->EnableInteraction();
}
