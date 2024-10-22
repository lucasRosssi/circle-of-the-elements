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

	if (const auto Combat = Data->Combats.Find(DifficultyClass))
	{
		return Combat->EnemyWaves;
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

TArray<TSoftObjectPtr<UWorld>> URegionInfo::GetRegionLocations(ERegion Region)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return TArray<TSoftObjectPtr<UWorld>>();

	return Data->Locations;
}

TSoftObjectPtr<UWorld> URegionInfo::GetRandomizedRegionLocation(
	ERegion Region,
	TArray<TSoftObjectPtr<UWorld>> LevelsToExclude
	)
{
	TArray<TSoftObjectPtr<UWorld>> Levels = GetRegionLocations(Region);
	if (Levels.IsEmpty()) return nullptr;

	if (!LevelsToExclude.IsEmpty() && LevelsToExclude.Num() < Levels.Num())
	{
		for (auto Level : LevelsToExclude)
		{
			Levels.RemoveSingle(Level);
		}
	}

	const int32 Index = FMath::RandRange(0, Levels.Num() - 1);

	return Levels[Index];
}

TSoftObjectPtr<UWorld> URegionInfo::GetRandomizedInitialLocation(ERegion Region)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return nullptr;

	TArray<TSoftObjectPtr<UWorld>> InitialLevels = Data->InitialLocations;
	if (InitialLevels.IsEmpty()) return nullptr;

	const int32 Index = FMath::RandRange(0, InitialLevels.Num() - 1);

	return InitialLevels[Index];
}

TSoftObjectPtr<UWorld> URegionInfo::GetBossArena(ERegion Region)
{
	const FRegionData* Data = GetRegionData(Region);

	if (!Data) return nullptr;

	return Data->BossArena;
}
