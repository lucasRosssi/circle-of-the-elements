// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "LocationManager.generated.h"

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

  UPROPERTY(BlueprintAssignable)
  FOnInitLocation OnInitLocationDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnExitLocation OnExitLocationDelegate;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion NextRegion = ERegion::Undefined;

private:
  TArray<TSoftObjectPtr<UWorld>> SelectedLocations;
  TSoftObjectPtr<UWorld> PrevLocation;
  TSoftObjectPtr<UWorld> CurrentLocation;

  UPROPERTY()
  TArray<AActor*> CameraBoundaryActors;

  bool bWillExitRegion = false;
};
