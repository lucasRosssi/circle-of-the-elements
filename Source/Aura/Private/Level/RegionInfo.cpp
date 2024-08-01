// Copyright Lucas Rossi


#include "Level/RegionInfo.h"

#include "Aura/AuraLogChannels.h"

FRegionData* URegionInfo::GetRegionData(ERegion Region)
{
	return RegionData.Find(Region);
}

TArray<FEnemyWave> URegionInfo::GetEnemyWaves(
	ERegion Region,
	FGameplayTag DifficultyClass
	)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return TArray<FEnemyWave>();

	if (const auto Encounter = Data->Encounters.Find(DifficultyClass))
	{
		return Encounter->EnemyWaves;
	}

	return TArray<FEnemyWave>();
}

TArray<FEnemyWave> URegionInfo::GetRandomizedEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass,
		int32 Amount
		)
{
	if (Amount < 1)
	{
		UE_LOG(LogAura, Error, TEXT("GetRandomizedEnemyWaves needs an amount greater than 0!"));
		return TArray<FEnemyWave>();
	}
	
	TArray<FEnemyWave> EnemyWaves = GetEnemyWaves(Region, DifficultyClass);
	if (EnemyWaves.IsEmpty()) return EnemyWaves;

	TArray<FEnemyWave> RandomWaves;
	for (int32 i = 1; i <= Amount; i++)
	{
		const int32 Index = FMath::RandRange(0, EnemyWaves.Num() - 1);

		if (EnemyWaves.IsValidIndex(Index))
		{
			RandomWaves.Add(EnemyWaves[Index]);
			EnemyWaves.RemoveAt(Index);
		}
	}

	return RandomWaves;
}

TArray<ULevel*> URegionInfo::GetRegionLevels(ERegion Region, EEntrancePosition EntrancePosition)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return TArray<ULevel*>();

	if (const auto AvailableLevels = Data->Locations.Find(EntrancePosition))
	{
		return AvailableLevels->Levels;
	}

	return TArray<ULevel*>();
}

ULevel* URegionInfo::GetRandomizedRegionLevel(
	ERegion Region,
	EEntrancePosition EntrancePosition,
	TArray<ULevel*> LevelsToExclude
	)
{
	TArray<ULevel*> Levels = GetRegionLevels(Region, EntrancePosition);
	if (Levels.IsEmpty()) return nullptr;

	if (!LevelsToExclude.IsEmpty())
	{
		for (auto Level : LevelsToExclude)
		{
			Levels.RemoveSingle(Level);
		}
	}

	const int32 Index = FMath::RandRange(0, Levels.Num() - 1);

	return Levels[Index];
}
