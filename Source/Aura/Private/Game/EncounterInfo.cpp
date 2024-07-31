// Copyright Lucas Rossi


#include "Game/EncounterInfo.h"

#include "Aura/AuraLogChannels.h"

FEncounterData* UEncounterInfo::GetEncounterData(ERegion Region)
{
	return EncounterData.Find(Region);
}

TArray<FEnemyWave> UEncounterInfo::GetEnemyWaves(
	ERegion Region,
	FGameplayTag DifficultyClass
	)
{
	const FEncounterData* Data = GetEncounterData(Region);

	if (!Data) return TArray<FEnemyWave>();

	if (const auto Encounter = Data->EncountersPerDifficulty.Find(DifficultyClass))
	{
		return Encounter->EnemyWaves;
	}

	return TArray<FEnemyWave>();
}

TArray<FEnemyWave> UEncounterInfo::GetRandomizedEnemyWaves(
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
