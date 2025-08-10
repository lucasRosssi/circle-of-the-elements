// Copyright Lucas Rossi


#include "Managers/LocationManager.h"

#include "Algo/RandomShuffle.h"
#include "Aura/AuraMacros.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Data/RegionInfo.h"
#include "Utils/AuraSystemsLibrary.h"
#include "Utils/UtilityLibrary.h"

void ULocationManager::GenerateLocation()
{
  // TODO: add generation logic
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
  Region = UAuraSystemsLibrary::GetCurrentRegion(this);
  Location = UAuraSystemsLibrary::GetCurrentLocation(this);

  PlacePlayerInStartingPoint();
  OnInitLocationDelegate.Broadcast();
}

void ULocationManager::ExitLocation()
{
  CameraBoundaryActors.Empty();
  OnExitLocationDelegate.Broadcast();
}

int32 ULocationManager::GetCurrentRegionRecommendedLevel()
{
  if (Region == ERegion::Undefined) return 1;

  const URegionInfo* RegionInfo = UAuraSystemsLibrary::GetRegionInfo(GetOwner());
  if (!RegionInfo) return 1;

  const FRegionData* RegionData = RegionInfo->GetRegionData(Region);
  if (!RegionData) return 1;

  return RegionInfo->GetRegionData(Region)->RecommendedLevel;
}

FAreaData ULocationManager::GetAreaFromPool(TArray<FAreaData>& Pool, const TArray<FAreaData>& Source)
{
  if (Pool.IsEmpty())
  {
    Pool = Source;
    Algo::RandomShuffle(Pool);
  }

  GUARD(!Pool.IsEmpty(), FAreaData(), TEXT("Pool is empty!"))

  return Pool.Pop();
}

FAreaData ULocationManager::GetEntranceFromPool()
{
  return GetAreaFromPool(EntrancesPool, GetRegionInfo()->GetEntrances(Region));
}

FAreaData ULocationManager::GetDefaultArenasFromPool()
{
  return GetAreaFromPool(DefaultArenasPool, GetRegionInfo()->GetDefaultArenas(Region));
}

FAreaData ULocationManager::GetSpiritArenasFromPool()
{
  return GetAreaFromPool(SpiritArenasPool, GetRegionInfo()->GetSpiritArenas(Region));
}

FAreaData ULocationManager::GetBossArenasFromPool()
{
  return GetAreaFromPool(BossArenasPool, GetRegionInfo()->GetBossArenas(Region));
}

FAreaData ULocationManager::GetRewardAreasFromPool()
{
  return GetAreaFromPool(RewardAreasPool, GetRegionInfo()->GetRewardAreas(Region));
}

FAreaData ULocationManager::GetSpecialAreasFromPool()
{
  return GetAreaFromPool(SpecialAreasPool, GetRegionInfo()->GetSpecialAreas(Region));
}

FAreaData ULocationManager::GetExitsFromPool()
{
  return GetAreaFromPool(ExitsPool, GetRegionInfo()->GetExits(Region));
}

bool ULocationManager::IsCoordinateFree(const FIntPoint& Coordinate) const
{
  return !LocationLayout.Contains(Coordinate);
}

void ULocationManager::ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction)
{
  FromArea.OpenDirections.Add(Direction);

  const ECardinalDirection OppositeDir = UUtilityLibrary::GetOppositeDirection(Direction);

  ToArea.OpenDirections.Add(OppositeDir);
}

void ULocationManager::PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData)
{
  PrevCoordinate = CurrentCoordinate;
  LocationLayout.Add(Coordinate, AreaData);
  CurrentCoordinate = Coordinate;

  if (CurrentCoordinate == FIntPoint(0, 0)) return;

  FAreaData* PrevArea = LocationLayout.Find(PrevCoordinate);
  FAreaData* CurrentArea = LocationLayout.Find(CurrentCoordinate);
  GUARD(
    PrevArea && CurrentArea,,
    TEXT("Something went wrong with location generation, PrevArea or CurrentArea are invalid!")
  )

  const ECardinalDirection ConnectDirection = UUtilityLibrary::GetDirectionFromCoordinateOffset(
    CurrentCoordinate - PrevCoordinate
  );

  ConnectAreas(*PrevArea, *CurrentArea, ConnectDirection);
}


URegionInfo* ULocationManager::GetRegionInfo()
{
  if (!RegionInfo.IsValid())
  {
    RegionInfo = UAuraSystemsLibrary::GetRegionInfo(GetOwner());
  }

  GUARD(RegionInfo.IsValid(), nullptr, TEXT("RegionInfo is invalid!"))
  return RegionInfo.Get();
}
