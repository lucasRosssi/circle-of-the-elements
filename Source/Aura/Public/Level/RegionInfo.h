// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/Region.h"
#include "RegionInfo.generated.h"

struct FAvailableLevels;
enum class EGatePosition : uint8;
class AAuraEnemy;

USTRUCT(BlueprintType)
struct FAvailableLevels
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UWorld>> Levels;
};

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
struct FRegionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxEncounters = 1;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="DifficultyClass",	ForceInlineRow)
		)
	TMap<FGameplayTag, FEncounter> Encounters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EGatePosition, FAvailableLevels> Locations;
};

/**
 * 
 */
UCLASS()
class AURA_API URegionInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	FRegionData* GetRegionData(ERegion Region);

	TArray<FEnemyWave> GetEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass
		);

	TArray<FEnemyWave> GetRandomizedEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass,
		int32 Amount
		);

	TArray<TSoftObjectPtr<UWorld>> GetRegionLevels(
		ERegion Region,
		EGatePosition EntrancePosition
		);
	TSoftObjectPtr<UWorld> GetRandomizedRegionLevel(
		ERegion Region,
		EGatePosition EntrancePosition,
		TArray<TSoftObjectPtr<UWorld>> LevelsToExclude
		);
protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="Encounter",	ForceInlineRow)
		)
	TMap<ERegion, FRegionData> RegionData;

private:
};
