// Copyright Lucas Rossi


#include "Game/HomeGameMode.h"

#include "Game/AuraSaveGame.h"
#include "Managers/AbilityManager.h"
#include "Managers/UpgradeManager.h"
#include "Utils/AuraSystemsLibrary.h"

AHomeGameMode::AHomeGameMode()
{
  AbilityManager = CreateDefaultSubobject<UAbilityManager>("AbilityManager");
  UpgradeManager = CreateDefaultSubobject<UUpgradeManager>("UpgradeManager");
}

void AHomeGameMode::BeginPlay()
{
  Super::BeginPlay();

  if (UAuraSaveGame* SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(this))
  {
    if (SaveGame->bJustLoaded)
    {
      SaveGame->bJustLoaded = false;
    }
  }
}


