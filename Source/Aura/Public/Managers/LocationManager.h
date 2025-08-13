// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Data/RegionInfo.h"
#include "Enums/AreaType.h"
#include "Enums/Region.h"
#include "LocationManager.generated.h"

struct FAreaTypeData;
enum class ECardinalDirection : uint8;
class AEnemySpawner;
class URegionInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitArea);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitArea);


USTRUCT(BlueprintType)
struct FAreaInfo
{
  GENERATED_BODY()

  UPROPERTY()
  TArray<AEnemySpawner*> EnemySpawners;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API ULocationManager : public UAuraSystemComponent
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  void GenerateLocation();
  
  void PlacePlayerInArea(const FAreaData& AreaData);

  void InitLocation();
  UFUNCTION(BlueprintCallable)
  void InitArea();
  UFUNCTION(BlueprintCallable)
  void ExitArea(ECardinalDirection ExitDirection);

  TArray<AActor*> GetCameraBoundaryActors() const { return CameraBoundaryActors; }
  UFUNCTION(BlueprintCallable)
  void SetCameraBoundaryActors(const TArray<AActor*>& InActors) { CameraBoundaryActors = InActors; }

  int32 GetCurrentRegionRecommendedLevel();

  UFUNCTION(BlueprintPure)
  TMap<FIntPoint, FAreaData> GetLocationLayout() const { return LocationLayout; }
  FIntPoint GetPlayerCoordinate() const { return PlayerCoordinate; }
  UFUNCTION(BlueprintPure)
  FAreaData& GetCurrentAreaRef() { return LocationLayout[PlayerCoordinate]; }
  
  UPROPERTY(BlueprintAssignable)
  FOnInitArea OnInitAreaDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnExitArea OnExitAreaDelegate;

protected:
  virtual void BeginPlay() override;
  
  FAreaData GetAreaFromPool(TArray<FAreaData>& Pool, const TArray<FAreaData>& Source);
  FAreaData GetAreaFromPoolByType(EAreaType AreaType);
  
  FAreaData GetEntranceFromPool();
  FAreaData GetDefaultArenaFromPool();
  FAreaData GetSpiritArenaFromPool();
  FAreaData GetBossArenaFromPool();
  FAreaData GetRewardAreaFromPool();
  FAreaData GetSpecialAreaFromPool();
  FAreaData GetExitFromPool();

  void HandleArenaGeneration(FAreaData& AreaData);

  FIntPoint Backtrack();
  FIntPoint GetNextCoordinate();

  void LoadArea(const FAreaData& AreaData);
  UFUNCTION()
  void OnAreaLoaded();
  
  void UnloadArea(const FAreaData& AreaData);
  UFUNCTION()
  void OnAreaUnloaded();

  bool IsCoordinateFree(const FIntPoint& Coordinate) const;

  // Returns a random adjacent free coordinate. Returns the same coordinate if there isn't any free.
  FIntPoint GetFreeAdjacentCoordinate(const FIntPoint& Coordinate) const;
  // Updates the open directions between two adjacent areas
  void ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction);
  // Updates a coordinate with area data
  void PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData);
  // Returns a random area type based on their probability weights and min and max counts in a location
  EAreaType PickRandomAreaType(const TMap<EAreaType, FAreaTypeData>& AreaTypesData, const TMap<EAreaType, int32>& PlacedCount) const;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(DisplayName="Active"))
  bool bPCGActive = true;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(ClampMin=3, UIMin=3))
  int32 MinAreas = 10;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(ClampMin=3, UIMin=3))
  int32 MaxAreas = 20;
  /*
   When true, enables backtracking during generation even if there are free adjacent coordinates.
   
   * Backtrack always triggers when there isn't any free adjacent coordinate, even if this is false.
   */
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG")
  bool bBacktrackEnabled = true;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|PCG",
    meta=(ClampMin=0.f, UIMin=0.f, ClampMax=1.f, UIMax=1.f, EditCondition="bBacktrackEnabled", EditConditionHides)
    )
  float BacktrackChance = 0.1f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|PCG",
    meta=(ClampMin=3, UIMin=3, EditCondition="bBacktrackEnabled", EditConditionHides)
    )
  int32 MinAreaCountToBacktrack = 3;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|PCG",
    meta=(ClampMin=3, UIMin=3, EditCondition="bBacktrackEnabled", EditConditionHides)
    )
  int32 MaxAreaCountToBacktrack = 17;

private:
  URegionInfo* GetRegionInfo();
  TWeakObjectPtr<URegionInfo> RegionInfo;
  
  TArray<FAreaData> EntrancesPool;
  TArray<FAreaData> DefaultArenasPool;
  TArray<FAreaData> SpiritArenasPool;
  TArray<FAreaData> BossArenasPool;
  TArray<FAreaData> RewardAreasPool;
  TArray<FAreaData> SpecialAreasPool;
  TArray<FAreaData> ExitsPool;

  int32 TotalAreas = 0;
  FIntPoint PrevCoordinate = FIntPoint(0, 0);
  FIntPoint CurrentCoordinate = FIntPoint(0, 0);
  int32 AreasCount = 0;
  int32 ArenaLevel = 0;
  
  TMap<FIntPoint, FAreaData> LocationLayout;

  FIntPoint PrevPlayerCoordinate = FIntPoint(0, 0);
  FIntPoint PlayerCoordinate = FIntPoint(0, 0);
  ECardinalDirection LastExit;

  UPROPERTY()
  TArray<AActor*> CameraBoundaryActors;
};
