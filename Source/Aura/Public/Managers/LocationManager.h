// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "LocationManager.generated.h"

enum class ECardinalDirection : uint8;
struct FAreaData;
class AEnemySpawner;
class URegionInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLocation);


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
  void GenerateLocation();
  void PlacePlayerInStartingPoint();

  UFUNCTION(BlueprintCallable)
  void InitLocation();
  UFUNCTION(BlueprintCallable)
  void ExitLocation();

  TArray<AActor*> GetCameraBoundaryActors() const { return CameraBoundaryActors; }
  UFUNCTION(BlueprintCallable)
  void SetCameraBoundaryActors(const TArray<AActor*>& InActors) { CameraBoundaryActors = InActors; }

  int32 GetCurrentRegionRecommendedLevel();

  UPROPERTY(BlueprintAssignable)
  FOnInitLocation OnInitLocationDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnExitLocation OnExitLocationDelegate;

protected:
  FAreaData GetAreaFromPool(TArray<FAreaData>& Pool, const TArray<FAreaData>& Source);
  
  FAreaData GetEntranceFromPool();
  FAreaData GetDefaultArenasFromPool();
  FAreaData GetSpiritArenasFromPool();
  FAreaData GetBossArenasFromPool();
  FAreaData GetRewardAreasFromPool();
  FAreaData GetSpecialAreasFromPool();
  FAreaData GetExitsFromPool();

  bool IsCoordinateFree(const FIntPoint& Coordinate) const;
  void ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction);
  void PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData);
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;
  FName Location = NAME_None;

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

  TMap<FIntPoint, FAreaData> LocationLayout;

  UPROPERTY()
  TArray<AActor*> CameraBoundaryActors;

  bool bWillExitLocation = false;
};
