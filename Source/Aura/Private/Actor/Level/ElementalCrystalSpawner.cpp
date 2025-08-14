// Copyright Lucas Rossi


#include "Actor/Level/ElementalCrystalSpawner.h"

#include "Actor/Level/LocationReward.h"
#include "Components/InteractComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
#include "Managers/RewardManager.h"
#include "Utils/AuraSystemsLibrary.h"

AElementalCrystalSpawner::AElementalCrystalSpawner()
{
  PrimaryActorTick.bCanEverTick = false;

  SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
  SetRootComponent(SceneRoot);
  SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpawnerMesh");
  SpawnerMesh->SetupAttachment(GetRootComponent());
}

void AElementalCrystalSpawner::BeginPlay()
{
  Super::BeginPlay();

  OriginalLocation = GetActorLocation();

  SpawnElementCrystal();

  UCombatManager* CombatManager = UAuraSystemsLibrary::GetCombatManager(this);
  if (CombatManager)
  {
    CombatManager->OnCombatFinishedDelegate.AddDynamic(this, &AElementalCrystalSpawner::Enable);
  }
}

void AElementalCrystalSpawner::Enable()
{
  if (Crystal)
  {
    Crystal->GetInteractComponent_Implementation()->EnableInteraction();
  }
  
  if (ActivationMode == ERewardSpawnerActivationMode::Movement) return;
  
  // TODO: use Chaos Fracture to break the mesh
}

void AElementalCrystalSpawner::SpawnElementCrystal()
{
  URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
  ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(this);

  if (!RewardManager || !LocationManager) return;
  if (LocationManager->GetCurrentAreaRef().bRewardSpawned) return;

  Crystal = RewardManager->SpawnReward(GetActorLocation());
  Crystal->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
  LocationManager->GetCurrentAreaRef().bRewardSpawned = true;
}
