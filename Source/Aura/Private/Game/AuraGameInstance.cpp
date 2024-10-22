// Copyright Lucas Rossi


#include "Game/AuraGameInstance.h"

#include "Character/Data/HeroInfo.h"

void UAuraGameInstance::SaveHeroData()
{
}

FHeroData UAuraGameInstance::GetCurrentHeroData() const
{
  return HeroInfo->GetHeroData(CurrentHeroName);
}
