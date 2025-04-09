// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "LocationManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLocation);

class AEnemySpawner;

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
  // UFUNCTION(BlueprintPure)
  // TSoftObjectPtr<UWorld> GetNextLocation(ERegion InRegion);
  // UFUNCTION(BlueprintPure)
  // TSoftObjectPtr<UWorld> GetInitialLocation(ERegion InRegion);

  void PlacePlayerInStartingPoint();

  UFUNCTION(BlueprintCallable)
  void InitLocation();
  UFUNCTION(BlueprintCallable)
  void ExitLocation();

  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetCurrentLocation() const { return CurrentLocation; }

  TArray<AActor*> GetCameraBoundaryActors() const { return CameraBoundaryActors; }
  UFUNCTION(BlueprintCallable)
  void SetCameraBoundaryActors(const TArray<AActor*>& InActors) { CameraBoundaryActors = InActors; }

  int32 GetCurrentLocationRecommendedLevel();

  UPROPERTY(BlueprintAssignable)
  FOnInitLocation OnInitLocationDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnExitLocation OnExitLocationDelegate;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;
  FName Location = NAME_None;

private:
  TArray<TSoftObjectPtr<UWorld>> SelectedLocations;
  TSoftObjectPtr<UWorld> PrevLocation;
  TSoftObjectPtr<UWorld> CurrentLocation;

  UPROPERTY()
  TArray<AActor*> CameraBoundaryActors;

  bool bWillExitRegion = false;
};
