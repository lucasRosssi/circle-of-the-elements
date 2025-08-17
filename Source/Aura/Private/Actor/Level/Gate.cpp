// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "NiagaraComponent.h"
#include "Aura/AuraMacros.h"
#include "Components/BoxComponent.h"
#include "Components/InteractComponent.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
#include "Utils/AuraSystemsLibrary.h"

AGate::AGate()
{
  GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
  SetRootComponent(GateMesh);
  InteractBox = CreateDefaultSubobject<UBoxComponent>("InteractBox");
  InteractBox->SetupAttachment(GetRootComponent());
  BlockerComponent = CreateDefaultSubobject<UBoxComponent>("BlockerComponent");
  BlockerComponent->SetupAttachment(GetRootComponent());
  BlockerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  BlockerComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
  BlockerComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
  BarrierMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrierMesh");
  BarrierMesh->SetupAttachment(GetRootComponent());
  BarrierNiagara = CreateDefaultSubobject<UNiagaraComponent>("BarrierNiagara");
  BarrierNiagara->SetupAttachment(GetRootComponent());
  
  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetInteractAreaComponent(InteractBox);
}

void AGate::Interact_Implementation(const AController* Controller)
{
  GUARD(GetLocationManager(),, TEXT("LocationManager not found!"))

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

  
}

void AGate::Open()
{
  BarrierNiagara->Deactivate();
  BlockerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Enable();
}

void AGate::Close()
{
  BarrierNiagara->Activate();
  BlockerComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  InteractComponent->DisableInteraction();
}

void AGate::Enable()
{
  if (!bActive) return;

  InteractComponent->EnableInteraction();
}

void AGate::SetIsActive(bool bIsActive)
{
  bActive = bIsActive;
  OnActiveToggle(bIsActive);

  if (!GetLocationManager()->GetCurrentAreaRef().bCombatFinished && bActive)
  {
    UCombatManager* CombatManager = UAuraSystemsLibrary::GetCombatManager(this);
    if (CombatManager)
    {
      CombatManager->OnCombatStartedDelegate.AddDynamic(this, &AGate::Close);
      CombatManager->OnCombatFinishedDelegate.AddDynamic(this, &AGate::Open);
    }
  }
}

void AGate::OnActiveToggle_Implementation(bool bIsActive)
{
  
}

ULocationManager* AGate::GetLocationManager()
{
  if (!LocationManager.IsValid())
  {
    LocationManager = UAuraSystemsLibrary::GetLocationManager(this);
  }
  
  return LocationManager.Get();
}
