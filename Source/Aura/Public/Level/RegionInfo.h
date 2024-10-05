// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/Region.h"
#include "RegionInfo.generated.h"

struct FAvailableLevels;
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
	TArray<TSoftObjectPtr<UWorld>> Locations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UWorld>> InitialLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> BossArena;
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

	UFUNCTION(BlueprintCallable)
	TArray<FEnemyWave> GetEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass
		);

	UFUNCTION(BlueprintCallable)
	TArray<FEnemyWave> GetRandomizedEnemyWaves(
		ERegion Region, 
		FGameplayTag DifficultyClass,
		int32 Amount
		);

	UFUNCTION(BlueprintCallable)
	TArray<TSoftObjectPtr<UWorld>> GetRegionLocations(
		ERegion Region
		);
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetRandomizedRegionLocation(
		ERegion Region,
		TArray<TSoftObjectPtr<UWorld>> LevelsToExclude
		);
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetRandomizedInitialLocation(ERegion Region);

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetBossArena(ERegion Region);
	
protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="Encounter",	ForceInlineRow)
		)
	TMap<ERegion, FRegionData> RegionData;

private:

};
