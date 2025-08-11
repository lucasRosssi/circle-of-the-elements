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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitLocation);
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
  
  void PlacePlayerInStartingPoint();
  
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

  UPROPERTY(BlueprintAssignable)
  FOnInitArea OnInitLocation;
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

  void LoadArea(const FAreaData& AreaData);
  void OnAreaLoaded();
  void UnloadArea(const FAreaData& AreaData);
  void OnAreaUnloaded();

  bool IsCoordinateFree(const FIntPoint& Coordinate) const;
  FIntPoint GetAdjacentFreeCoordinate(const FIntPoint& Coordinate) const;
  void ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction);
  void PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData);
  EAreaType PickRandomAreaType(const TMap<EAreaType, FAreaTypeData>& AreaTypesData, const TMap<EAreaType, int32>& PlacedCount) const;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(DisplayName="Active"))
  bool bPCGActive = true;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(ClampMin=3, UIMin=3))
  int32 MinAreas = 10;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|PCG", meta=(ClampMin=3, UIMin=3))
  int32 MaxAreas = 20;

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
