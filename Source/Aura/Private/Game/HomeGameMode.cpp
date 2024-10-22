// Copyright Lucas Rossi


#include "Game/HomeGameMode.h"

#include "Managers/AbilityManager.h"
#include "Managers/UpgradeManager.h"

AHomeGameMode::AHomeGameMode()
{
  AbilityManager = CreateDefaultSubobject<UAbilityManager>("AbilityManager");
  UpgradeManager = CreateDefaultSubobject<UUpgradeManager>("UpgradeManager");
}
