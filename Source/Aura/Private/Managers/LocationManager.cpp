// Copyright Lucas Rossi


#include "Managers/LocationManager.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"
#include "Managers/CombatManager.h"
#include "Utils/AuraSystemsLibrary.h"

TSoftObjectPtr<UWorld> ULocationManager::GetNextLocation(
  ERegion InRegion
)
{
  // UAuraSystemsLibrary::SaveGameData(this);

  if (bWillExitRegion)
  {
    return nullptr;
  }

  URegionInfo* RegionInfo = UAuraSystemsLibrary::GetRegionInfo(this);
  const int32 CombatsCount = UAuraSystemsLibrary::GetCombatManager(this)->GetCombatsCount();
  TSoftObjectPtr<UWorld> Location;
  if (CombatsCount < RegionInfo->GetRegionData(InRegion)->MaxCombats)
  {
    Location = RegionInfo->GetRandomizedRegionLocation(
      InRegion,
      SelectedLocations
    );
  }
  else
  {
    Location = RegionInfo->GetBossArena(InRegion);
    bWillExitRegion = true;
  }

  SelectedLocations.Add(Location);
  PrevLocation = CurrentLocation;
  CurrentLocation = Location;

  return Location;
}

TSoftObjectPtr<UWorld> ULocationManager::GetInitialLocation(ERegion InRegion)
{
  const TSoftObjectPtr<UWorld> Location = UAuraSystemsLibrary::GetRegionInfo(this)
    ->GetRandomizedInitialLocation(InRegion);

  CurrentLocation = Location;
  return Location;
}

void ULocationManager::PlacePlayerInStartingPoint()
{
  TArray<AActor*> PlayerStarts;
  UGameplayStatics::GetAllActorsOfClass(
    this,
    APlayerStart::StaticClass(),
    PlayerStarts
  );

  if (PlayerStarts.IsEmpty()) return;

  const APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[0]);

  APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  Player->SetActorTransform(
    PlayerStart->GetActorTransform(),
    false,
    nullptr,
    ETeleportType::ResetPhysics
  );
}

void ULocationManager::InitLocation()
{
  PlacePlayerInStartingPoint();
  OnInitLocationDelegate.Broadcast();
}

void ULocationManager::ExitLocation()
{
  CameraBoundaryActors.Empty();
  OnExitLocationDelegate.Broadcast();
}
