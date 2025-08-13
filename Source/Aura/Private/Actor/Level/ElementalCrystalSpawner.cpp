// Copyright Lucas Rossi


#include "Actor/Level/ElementalCrystalSpawner.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
#include "Managers/RewardManager.h"
#include "Utils/AuraSystemsLibrary.h"

AElementalCrystalSpawner::AElementalCrystalSpawner()
{
  PrimaryActorTick.bCanEverTick = false;
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
    // TODO: use Chaos Fracture to break the mesh
  FVector TargetLocation = OriginalLocation;
  TargetLocation.Z += ZOffsetWhenEnabled;

  const FLatentActionInfo LatentActionInfo;
  UKismetSystemLibrary::MoveComponentTo(
    GetRootComponent(),
    TargetLocation,
    GetActorRotation(),
    false,
    false,
    TimeToMove,
    false,
    EMoveComponentAction::Move,
    LatentActionInfo
  );
}

void AElementalCrystalSpawner::SpawnElementCrystal()
{
  URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
  ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(this);

  if (!RewardManager || !LocationManager) return;
  if (LocationManager->GetCurrentAreaRef().bRewardSpawned) return;

  RewardManager->SpawnReward(GetActorLocation());
  LocationManager->GetCurrentAreaRef().bRewardSpawned = true;
}
