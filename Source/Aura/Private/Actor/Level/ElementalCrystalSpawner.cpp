// Copyright Lucas Rossi


#include "Actor/Level/ElementalCrystalSpawner.h"

#include "Managers/RewardManager.h"
#include "Utils/AuraSystemsLibrary.h"

AElementalCrystalSpawner::AElementalCrystalSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AElementalCrystalSpawner::BeginPlay()
{
	Super::BeginPlay();

  SpawnElementCrystal();
}

void AElementalCrystalSpawner::SpawnElementCrystal()
{
  URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);

  if (!RewardManager) return;

  RewardManager->SpawnReward(GetActorLocation());
}
