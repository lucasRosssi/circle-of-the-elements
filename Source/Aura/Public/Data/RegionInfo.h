// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/AreaType.h"
#include "Enums/Region.h"
#include "RegionInfo.generated.h"

enum class ECardinalDirection : uint8;
class UGameplayEffect;
struct FAvailableLevels;
class AAuraEnemy;

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<AAuraEnemy> EnemyClass;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20))
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
struct FArenaDifficultyData
{
  GENERATED_BODY()

  // Total points allowed for the system to spend on creating random encounters
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  float DifficultyPoints = 1.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20))
  int32 MinLevel = 1;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20))
  int32 MaxLevel = 1;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1))
  float MinWavePoints = 1.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1))
  float MaxWavePoints = 1.f;

  // Map of available enemies to pick and their probability weight
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ForceInlineRow, Categories="Enemies"))
  TMap<FGameplayTag, float> EnemiesProbabilityWeight;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ForceInlineRow, Categories="Enemies", ClampMin=1, UIMin=1))
  TMap<FGameplayTag, int32> EnemiesRestrictionsPerWave;
};

USTRUCT(BlueprintType)
struct FAreaData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> World;

  FIntPoint Coordinate;

  EAreaType AreaType;

  TSet<ECardinalDirection> OpenDirections;

  int32 ArenaLevel = -1;

  bool IsValid() const { return World.IsValid(); }

  bool IsEntrance() const { return AreaType == EAreaType::Entrance; }
  bool IsArena() const { return AreaType == EAreaType::DefaultArena || AreaType == EAreaType::SpiritArena || AreaType == EAreaType::BossArena; }
  bool IsRewardArea() const { return AreaType == EAreaType::RewardArea; }
  bool IsSpecialArea() const { return AreaType == EAreaType::SpecialArea; }
  bool IsExit() const { return AreaType == EAreaType::Exit; }

  FString GetAreaName() const { return World.GetAssetName();}
};

USTRUCT(BlueprintType)
struct FAreaTypeData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  float Rarity = 0.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
  int32 MinPerLocation = 0;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
  int32 MaxPerLocation = 0;
};

USTRUCT(BlueprintType)
struct FRegionData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20))
  int32 RecommendedLevel = 1;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FArenaDifficultyData> ArenaDifficultyData;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGameplayTagContainer Bosses;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> Entrances;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> DefaultArenas;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> SpiritArenas;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> BossArenas;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> RewardAreas;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> SpecialAreas;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FAreaData> Exits;
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

  const FRegionData* GetRegionData(ERegion Region) const;
  TMap<EAreaType, FAreaTypeData> GetAreaTypesData() const { return AreaTypesData; }

  TArray<FAreaData> GetEntrances(ERegion Region) const;
  TArray<FAreaData> GetDefaultArenas(ERegion Region) const;
  TArray<FAreaData> GetSpiritArenas(ERegion Region) const;
  TArray<FAreaData> GetBossArenas(ERegion Region) const;
  TArray<FAreaData> GetRewardAreas(ERegion Region) const;
  TArray<FAreaData> GetSpecialAreas(ERegion Region) const;
  TArray<FAreaData> GetExits(ERegion Region) const;

  FArenaDifficultyData GetArenaDifficultyData(ERegion Region, int32 ArenaLevel) const;

  FGameplayTagContainer GetRegionBosses(ERegion Region) const;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> HomeLevel;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSoftObjectPtr<UWorld> NewGameLevel;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(ForceInlineRow)
  )
  TMap<ERegion, FRegionData> RegionData;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(ForceInlineRow)
  )
  TMap<EAreaType, FAreaTypeData> AreaTypesData;

private:
};
