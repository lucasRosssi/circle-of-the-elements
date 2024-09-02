// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "Game/AuraGameModeBase.h"
#include "LocationManager.generated.h"


class UAuraGameInstance;
enum class EGatePosition : uint8;
class AAuraGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API ULocationManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetNextLocation(ERegion InRegion, EGatePosition EntrancePosition);
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetInitialLocation(ERegion InRegion);

	UFUNCTION(BlueprintCallable)
	void PlacePlayerInStartingPoint();
	
	UFUNCTION(BlueprintCallable)
	void ExitLocation(EGatePosition NextGatePosition);

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLocation() const { return CurrentLocation; }

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

	bool bWillExitRegion = false;
	
};
