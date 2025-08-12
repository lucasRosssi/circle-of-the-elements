// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Aura/AuraMacros.h"
#include "Components/InteractComponent.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
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
  ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(this);
  GUARD(LocationManager,, TEXT("LocationManager not found!"))

  LocationManager->ExitArea(Direction);
  
  OnInteracted(Controller);
}

UInteractComponent* AGate::GetInteractComponent_Implementation() const
{
  return InteractComponent;
}

void AGate::BeginPlay()
{
  Super::BeginPlay();

  UAuraSystemsLibrary::GetOnCombatFinishedDelegate(this).AddDynamic(this, &AGate::OnCombatFinished);
}

void AGate::OnCombatFinished()
{
  Enable();
}

void AGate::Enable()
{
  if (!bActive) return;

  InteractComponent->EnableInteraction();
}

void AGate::SetIsActive(bool bIsActive)
{
  bActive = bIsActive;
  GateMesh->SetVisibility(bActive);
}
