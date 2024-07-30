// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/Region.h"
#include "EncounterInfo.generated.h"

class AAuraEnemy;



USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AAuraEnemy>> Enemies;
};

USTRUCT(BlueprintType)
struct FEncounter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemyWave> EnemyWaves;
};

USTRUCT(BlueprintType)
struct FEncounterData
{
	GENERATED_BODY()

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="DifficultyClass",	ForceInlineRow)
		)
	TMap<FGameplayTag, FEncounter> EncountersPerDifficulty;

	
};

/**
 * 
 */
UCLASS()
class AURA_API UEncounterInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	FEncounterData* GetEncounterData(ERegion Region);

	TArray<FEnemyWave> GetEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass
		);

	TArray<FEnemyWave> GetRandomizedEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass,
		int32 Amount
		);

protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="Encounter",	ForceInlineRow)
		)
	TMap<ERegion, FEncounterData> EncounterData;

private:
};
