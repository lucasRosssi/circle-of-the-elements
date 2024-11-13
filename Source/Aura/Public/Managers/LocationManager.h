// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "LocationManager.generated.h"

class ADungeonGenerator;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLocation);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API ULocationManager : public UAuraSystemComponent
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetNextLocation(ERegion InRegion);
  UFUNCTION(BlueprintPure)
  TSoftObjectPtr<UWorld> GetInitialLocation(ERegion InRegion);

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

  ADungeonGenerator* GetDungeonGenerator();

  UPROPERTY(BlueprintAssignable)
  FOnInitLocation OnInitLocationDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnExitLocation OnExitLocationDelegate;

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion NextRegion = ERegion::Undefined;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Procedural Generation")
  bool bUseProceduralGeneration = true;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Procedural Generation")
  TSubclassOf<ADungeonGenerator> DungeonGeneratorClass;

private:
  TArray<TSoftObjectPtr<UWorld>> SelectedLocations;
  TSoftObjectPtr<UWorld> PrevLocation;
  TSoftObjectPtr<UWorld> CurrentLocation;

  UPROPERTY()
  TArray<AActor*> CameraBoundaryActors;

  bool bWillExitRegion = false;

  UPROPERTY()
  TObjectPtr<ADungeonGenerator> DungeonGenerator;
};
