// Copyright Lucas Rossi


#include "Managers/LocationManager.h"

#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Level/DungeonGenerator.h"
#include "Level/RegionInfo.h"
#include "Managers/CombatManager.h"
#include "Utils/AuraSystemsLibrary.h"

TSoftObjectPtr<UWorld> ULocationManager::GetNextLocation(
  ERegion InRegion
)
{
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
    
    const int32 LocationIndex = RegionInfo->FindLocationIndex(InRegion, Location);
    GetSaveGame()->LocationManager.Index = LocationIndex;
    GetSaveGame()->LocationManager.SelectedLocationsIndex.Add(LocationIndex);
  }
  else
  {
    Location = RegionInfo->GetBossArena(InRegion);
    bWillExitRegion = true;
    GetSaveGame()->LocationManager.Index = BOSS_LOCATION;
  }

  SelectedLocations.Add(Location);
  PrevLocation = CurrentLocation;
  CurrentLocation = Location;
  
  return Location;
}

TSoftObjectPtr<UWorld> ULocationManager::GetInitialLocation(ERegion InRegion)
{
  URegionInfo* RegionInfo = UAuraSystemsLibrary::GetRegionInfo(this);
  
  TSoftObjectPtr<UWorld> Location;
  if (GetSaveGame())
  {
    if (SaveGame->LocationManager.Index < 0)
    {
      Location = RegionInfo->GetRandomizedInitialLocation(InRegion);
    }
    else if (SaveGame->LocationManager.Index == BOSS_LOCATION)
    {
      Location = RegionInfo->GetBossArena(InRegion);
    }
    else
    {
      Location = RegionInfo->GetRegionLocationByIndex(InRegion, SaveGame->LocationManager.Index);
    }

    for (const int32 Index : SaveGame->LocationManager.SelectedLocationsIndex)
    {
      SelectedLocations.Add(RegionInfo->GetRegionLocations(InRegion)[Index]);
    }
  }
  else
  {
    Location = RegionInfo->GetRandomizedInitialLocation(InRegion);
  }

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

ADungeonGenerator* ULocationManager::GetDungeonGenerator()
{
  if (DungeonGenerator == nullptr)
  {
    const FVector Location = FVector::ZeroVector;
    const FRotator Rotation = FRotator::ZeroRotator;
    DungeonGenerator = Cast<ADungeonGenerator>(GetWorld()->SpawnActor(DungeonGeneratorClass, &Location, &Rotation));

    if (!DungeonGenerator)
    {
      UE_LOG(
        LogAura,
        Error,
        TEXT("Failed to spawn the Dungeon Generator! Check if DungeonGeneratorClass is set.")
      );
    }
  }

  return DungeonGenerator;
}

void ULocationManager::BeginPlay()
{
  Super::BeginPlay();

  if (bUseProceduralGeneration)
  {
    GetDungeonGenerator();
  }
}
