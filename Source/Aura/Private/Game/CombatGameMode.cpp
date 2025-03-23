// Copyright Lucas Rossi


#include "Game/CombatGameMode.h"

#include "Game/AuraSaveGame.h"
#include "Managers/AbilityManager.h"
#include "Managers/CombatManager.h"
#include "Managers/LocationManager.h"
#include "Managers/MatchManager.h"
#include "Managers/RewardManager.h"
#include "Managers/UpgradeManager.h"
#include "Utils/AuraSystemsLibrary.h"

ACombatGameMode::ACombatGameMode()
{
  AbilityManager = CreateDefaultSubobject<UAbilityManager>("Ability Manager");
  LocationManager = CreateDefaultSubobject<ULocationManager>("LocationManager");
  CombatManager = CreateDefaultSubobject<UCombatManager>("CombatManager");
  RewardManager = CreateDefaultSubobject<URewardManager>("RewardManager");
  UpgradeManager = CreateDefaultSubobject<UUpgradeManager>("UpgradeManager");
  MatchManager = CreateDefaultSubobject<UMatchManager>("MatchManager");
}

void ACombatGameMode::BeginPlay()
{
  Super::BeginPlay();

  GetOnCombatFinishedDelegate().AddDynamic(RewardManager, &URewardManager::SpawnReward);
}

void ACombatGameMode::LoadLevelInfo()
{
  LocationManager->InitLocation();
  CombatManager->SetCurrentCombatData();
  RewardManager->SetGatesRewards();

  if (UAuraSaveGame* SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(this))
  {
    if (SaveGame->bJustLoaded)
    {
      SaveGame->bJustLoaded = false;
    }
    else
    {
      UAuraSystemsLibrary::SaveCurrentGame(this);
    }
  }
}

int32 ACombatGameMode::GetEnemiesLevel() const
{
  return CombatManager->GetEnemiesLevel();
}

FOnExitLocation& ACombatGameMode::GetOnExitLocationDelegate()
{
  return LocationManager->OnExitLocationDelegate;
}

FOnCombatFinished& ACombatGameMode::GetOnCombatFinishedDelegate()
{
  return CombatManager->OnCombatFinishedDelegate;
}

FOnRewardTaken& ACombatGameMode::GetOnRewardTakenDelegate()
{
  return RewardManager->OnRewardTakenDelegate;
}
