// Copyright Lucas Rossi


#include "Character/Data/HeroInfo.h"

FHeroData UHeroInfo::GetHeroData(ECharacterName HeroName)
{
  if (const FHeroData* Data = HeroData.Find(HeroName))
  {
    return *Data;
  }

  UE_LOG(LogTemp, Error, TEXT("Data for Hero %hhd not found"), HeroName);

  return FHeroData();
}
