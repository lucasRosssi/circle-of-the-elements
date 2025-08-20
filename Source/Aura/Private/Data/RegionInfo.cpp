// Copyright Lucas Rossi


#include "Data/RegionInfo.h"

#include "Aura/AuraLogChannels.h"
#include "Aura/AuraMacros.h"

const FRegionData* URegionInfo::GetRegionData(ERegion Region) const
{
  const FRegionData* Data = RegionData.Find(Region);

  GUARD(Data != nullptr, nullptr, TEXT("Region data not found: %d"), Region)

  return Data;
}

TArray<FAreaData> URegionInfo::GetEntrances(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->Entrances;
}

TArray<FAreaData> URegionInfo::GetDefaultArenas(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->DefaultArenas;
}

TArray<FAreaData> URegionInfo::GetSpiritArenas(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->SpiritArenas;
}

TArray<FAreaData> URegionInfo::GetBossArenas(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->BossArenas;
}

TArray<FAreaData> URegionInfo::GetRewardAreas(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->RewardAreas;
}

TArray<FAreaData> URegionInfo::GetSpecialAreas(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->SpecialAreas;
}

TArray<FAreaData> URegionInfo::GetExits(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, TArray<FAreaData>(), TEXT("Region data not found: %d"), Region)

  return Data->Exits;
}

FArenaDifficultyData URegionInfo::GetArenaDifficultyData(ERegion Region, EAreaType ArenaType, int32 ArenaLevel) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, FArenaDifficultyData(), TEXT("Region data not found: %d"), Region)
  GUARD(
    Data->ArenaDifficultyData.IsValidIndex(ArenaLevel),
    Data->ArenaDifficultyData.Last(),
    TEXT("Difficulty data not found for arena level %d! Falling back to last level available (%d)"),
    ArenaLevel,
    Data->ArenaDifficultyData.Num() - 1
  )

  switch (ArenaType)
  {
  case EAreaType::SpiritArena:
    return Data->SpiritArenaDifficultyData[ArenaLevel];
  case EAreaType::DefaultArena:
  default:
    return Data->ArenaDifficultyData[ArenaLevel];
  }

}

FGameplayTagContainer URegionInfo::GetRegionBosses(ERegion Region) const
{
  const FRegionData* Data = GetRegionData(Region);

  GUARD(Data != nullptr, FGameplayTagContainer(), TEXT("Region data not found: %d"), Region)

  return Data->Bosses;
}
