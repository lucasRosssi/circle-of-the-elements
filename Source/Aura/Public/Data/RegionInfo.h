// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/CombatMode.h"
#include "Enums/Region.h"
#include "RegionInfo.generated.h"

class UGameplayEffect;
struct FAvailableLevels;
class AAuraEnemy;

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
  GENERATED_BODY()
	
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<AAuraEnemy> EnemyClass;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1,ClampMax=50,UIMax=50))
  int32 Level = 1;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TArray<TSubclassOf<UGameplayEffect>> Modifiers;
};

USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()
	
	UPROPERTY(
	  EditAnywhere,
	  BlueprintReadOnly,
	  meta=(TitleProperty="Lv.{Level} {EnemyClass}")
	  )
	TArray<FEnemySpawnData> Enemies;
};

USTRUCT(BlueprintType)
struct FDifficultyPointsData
{
  GENERATED_BODY()

  // Total points allowed for the system to spend on creating random encounters
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float DifficultyPoints = 1.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1,ClampMax=20,UIMax=20))
  int32 MinLevel = 1;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1,ClampMax=20,UIMax=20))
  int32 MaxLevel = 1;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1))
  float MinWavePoints = 1.f;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1))
  float MaxWavePoints = 1.f;

  // Map of available enemies to pick and their probability weight
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ForceInlineRow, Categories="Enemies"))
  TMap<FGameplayTag, float> EnemiesProbabilityWeight;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ForceInlineRow, Categories="Enemies", ClampMin=1,UIMin=1))
  TMap<FGameplayTag, int32> EnemiesRestrictionsPerWave;
};

USTRUCT(BlueprintType)
struct FCombat
{
  GENERATED_BODY()
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  ECombatMode Mode = ECombatMode::DifficultyPoints;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="Mode == ECombatMode::Defined", EditConditionHides))
  TArray<FEnemyWave> EnemyWaves;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="Mode == ECombatMode::DifficultyPoints", EditConditionHides))
  FDifficultyPointsData Data;
};

USTRUCT(BlueprintType)
struct FLocation
{
  GENERATED_BODY()
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> World;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1,UIMin=1,ClampMax=20,UIMax=20))
  int32 RecommendedLevel = 1;
  
  
  // Areas and their Enemy Waves
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(ForceInlineRow)
    )
  TMap<FName, FCombat> Combats;

  TArray<FName> GetAreas() const
  {
    TArray<FName> Areas;
    Combats.GenerateKeyArray(Areas);
  
    return Areas;
  }

  bool IsValid() const { return World.IsValid(); }
};

USTRUCT(BlueprintType)
struct FRegionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, FLocation> Locations;
};

/**
 * 
 */
UCLASS()
class AURA_API URegionInfo : public UDataAsset
{
	GENERATED_BODY()
public:
  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetNewGameLevel() const { return NewGameLevel; }
  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetHomeLevel() const { return HomeLevel; }
  
  // UFUNCTION(BlueprintPure)
  // TMap<ERegion, TSoftObjectPtr<UWorld>> GetLevels() { return Levels; }
  
	FRegionData* GetRegionData(ERegion Region);

  UFUNCTION(BlueprintPure)
  FLocation GetLocationData(FName LocationName, ERegion Region = ERegion::Undefined);
  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetLocationLevel(FName LocationName, ERegion Region = ERegion::Undefined);
  UFUNCTION(BlueprintPure)
  TArray<FName> GetLocationAreas(FName LocationName, ERegion Region = ERegion::Undefined);

	UFUNCTION(BlueprintCallable)
	TArray<FEnemyWave> GetEnemyWaves(
    FName AreaName,
    FName LocationName,
	  ERegion Region
		);

	// UFUNCTION(BlueprintCallable)
	// TArray<FEnemyWave> GetRandomizedEnemyWaves(
	//   ERegion Region,
 //    FName LocationName,
 //    FName AreaName
	// 	);

	UFUNCTION(BlueprintCallable)
	TMap<FName, FLocation> GetRegionLocations(
		ERegion Region
		);
	// UFUNCTION(BlueprintCallable)
	// TSoftObjectPtr<UWorld> GetRandomizedRegionLocation(
	// 	ERegion Region,
	// 	TArray<TSoftObjectPtr<UWorld>> LevelsToExclude
	// 	);
	// UFUNCTION(BlueprintCallable)
	// TSoftObjectPtr<UWorld> GetRandomizedInitialLocation(ERegion Region);

	// UFUNCTION(BlueprintCallable)
	// TSoftObjectPtr<UWorld> GetBossArena(ERegion Region);

  // int32 FindLocationIndex(ERegion Region, TSoftObjectPtr<UWorld> Location);

  // TSoftObjectPtr<UWorld> GetRegionLocationByIndex(ERegion Region, int32 Index);
	
protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> HomeLevel;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> NewGameLevel;

  // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  // TMap<ERegion, TSoftObjectPtr<UWorld>> Levels;
  
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(Categories="Combat",	ForceInlineRow)
		)
	TMap<ERegion, FRegionData> RegionData;


private:
};
