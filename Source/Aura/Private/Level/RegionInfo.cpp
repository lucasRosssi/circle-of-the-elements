// Copyright Lucas Rossi


#include "Level/RegionInfo.h"

#include "Aura/AuraLogChannels.h"

FRegionData* URegionInfo::GetRegionData(ERegion Region)
{
	return RegionData.Find(Region);
}

FLocation URegionInfo::GetLocationData(FName LocationName, ERegion Region)
{
  if (Region != ERegion::Undefined)
  {
    FRegionData* Data = RegionData.Find(Region);
    if (!Data) return FLocation();

    return *Data->Locations.Find(LocationName);
  }

  for (auto [ItRegion, ItRegionData] : RegionData)
  {
    if (const auto Location = ItRegionData.Locations.Find(LocationName))
    {
      return *Location;
    }
  }

  UE_LOG(LogAura, Error, TEXT("[RegionInfo] Location not found!"));

  return FLocation();
}

TSoftObjectPtr<UWorld> URegionInfo::GetLocationLevel(FName LocationName, ERegion Region)
{
  return GetLocationData(LocationName, Region).World;
}

TArray<FEnemyWave> URegionInfo::GetEnemyWaves(
  FName AreaName,
  FName LocationName,
  ERegion Region
	)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return TArray<FEnemyWave>();

  if (const auto LocationData = Data->Locations.Find(LocationName))
  {
    if (const auto Combat = LocationData->Combats.Find(AreaName))
    {
		  return Combat->EnemyWaves;
    }
  }

	return TArray<FEnemyWave>();
}

// TArray<FEnemyWave> URegionInfo::GetRandomizedEnemyWaves(
//   ERegion Region,
//   FName LocationName,
//   FName AreaName
// 	)
// {
// 	TArray<FEnemyWave> EnemyWaves = GetEnemyWaves(Region, LocationName, AreaName);
// 	if (EnemyWaves.IsEmpty()) return EnemyWaves;
//
// 	TArray<FEnemyWave> RandomWaves;
// 	for (int32 i = 1; i <= Amount; i++)
// 	{
// 		const int32 Index = FMath::RandRange(0, EnemyWaves.Num() - 1);
//
// 		if (EnemyWaves.IsValidIndex(Index))
// 		{
// 			RandomWaves.Add(EnemyWaves[Index]);
// 			EnemyWaves.RemoveAt(Index);
// 		}
// 	}
//
// 	return RandomWaves;
// }

TMap<FName, FLocation> URegionInfo::GetRegionLocations(ERegion Region)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return TMap<FName, FLocation>();

	return Data->Locations;
}

// TSoftObjectPtr<UWorld> URegionInfo::GetRandomizedRegionLocation(
// 	ERegion Region,
// 	TArray<TSoftObjectPtr<UWorld>> LevelsToExclude
// 	)
// {
// 	TArray<TSoftObjectPtr<UWorld>> Locations = GetRegionLocations(Region);
// 	if (Locations.IsEmpty()) return nullptr;
//
// 	if (!LevelsToExclude.IsEmpty() && LevelsToExclude.Num() < Locations.Num())
// 	{
// 		for (auto Level : LevelsToExclude)
// 		{
// 			Locations.RemoveSingle(Level);
// 		}
// 	}
//
// 	const int32 Index = FMath::RandRange(0, Locations.Num() - 1);
//
// 	return Locations[Index];
// }
//
// TSoftObjectPtr<UWorld> URegionInfo::GetRandomizedInitialLocation(ERegion Region)
// {
// 	const FRegionData* Data = GetRegionData(Region);
//
// 	if (!Data) return nullptr;
//
// 	TArray<TSoftObjectPtr<UWorld>> InitialLevels = Data->InitialLocations;
// 	if (InitialLevels.IsEmpty()) return nullptr;
//
// 	const int32 Index = FMath::RandRange(0, InitialLevels.Num() - 1);
//
// 	return InitialLevels[Index];
// }

// TSoftObjectPtr<UWorld> URegionInfo::GetBossArena(ERegion Region)
// {
// 	const FRegionData* Data = GetRegionData(Region);
//
// 	if (!Data) return nullptr;
//
// 	return Data->BossArena;
// }

// int32 URegionInfo::FindLocationIndex(ERegion Region, TSoftObjectPtr<UWorld> Location)
// {
//   return GetRegionLocations(Region).IndexOfByPredicate(
//     [Location](const TSoftObjectPtr<UWorld>& Item)
//     {
//       return Item == Location;
//     }
//     );
// }

// TSoftObjectPtr<UWorld> URegionInfo::GetRegionLocationByIndex(ERegion Region, int32 Index)
// {
//   return GetRegionLocations(Region)[Index];
// }
