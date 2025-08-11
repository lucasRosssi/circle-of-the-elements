// Copyright Lucas Rossi


#include "Managers/LocationManager.h"

#include "Algo/RandomShuffle.h"
#include "Aura/AuraMacros.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Data/RegionInfo.h"
#include "Managers/CombatManager.h"
#include "Utils/AuraSystemsLibrary.h"
#include "Utils/UtilityLibrary.h"

void ULocationManager::GenerateLocation()
{
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Starting location generation..."))
  
  const int32 NumAreas = FMath::RandRange(MinAreas, MaxAreas);
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Location will have %d areas."), NumAreas)
  
  const TMap<EAreaType, FAreaTypeData>& AreaTypesData = GetRegionInfo()->GetAreaTypesData();

  TMap<EAreaType, int32> GeneratedAreasCount;
  TArray<FAreaData> CachedAreas;
  
  FIntPoint NextCoordinate = FIntPoint(0, 0);
  PlaceAreaAtCoordinate(NextCoordinate, GetEntranceFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated Entrance at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)
  
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Randomizing area types..."))
  for (int32 i = 2; i < NumAreas - 1; i++)
  {
    EAreaType ChosenType = PickRandomAreaType(AreaTypesData, GeneratedAreasCount);
    CachedAreas.Add(GetAreaFromPoolByType(ChosenType));

    GeneratedAreasCount.FindOrAdd(ChosenType)++;
  }
  
  Algo::RandomShuffle(CachedAreas);

  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generating areas..."))
  for (int32 i = 0; i < CachedAreas.Num(); i++)
  {
    NextCoordinate = GetAdjacentFreeCoordinate(CurrentCoordinate);
    PlaceAreaAtCoordinate(NextCoordinate, CachedAreas[i]);
    UE_LOG(
      LogAura,
      Display,
      TEXT("[LocationManager] Generated %s at [%d,%d]"),
      *UUtilityLibrary::EnumToString(CachedAreas[i].AreaType),
      CurrentCoordinate.X,
      CurrentCoordinate.Y
    )
  }

  NextCoordinate = GetAdjacentFreeCoordinate(CurrentCoordinate);
  PlaceAreaAtCoordinate(NextCoordinate, GetBossArenaFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated BossArena at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)
  
  NextCoordinate = GetAdjacentFreeCoordinate(CurrentCoordinate);
  PlaceAreaAtCoordinate(NextCoordinate, GetExitFromPool());
  UE_LOG(LogAura, Display, TEXT("[LocationManager] Generated Exit at [%d,%d]"), NextCoordinate.X, NextCoordinate.Y)

  UE_LOG(LogAura, Display, TEXT("[LocationManager] Location successfully generated!"));
}

EAreaType ULocationManager::PickRandomAreaType(
  const TMap<EAreaType, FAreaTypeData>& AreaTypesData,
  const TMap<EAreaType, int32>& PlacedCount
) const
{
  struct FWeightedType { EAreaType Type; float Weight; };
  TArray<FWeightedType> WeightedList;
  float TotalWeight = 0.f;

  for (const auto& Pair : AreaTypesData)
  {
    const EAreaType Type = Pair.Key;
    const FAreaTypeData& Data = Pair.Value;

    // Skip fixed types
    if (Type == EAreaType::Entrance || Type == EAreaType::BossArena || Type == EAreaType::Exit)
      continue;

    if (Data.MinPerLocation > 0 && PlacedCount.FindRef(Type) < Data.MinPerLocation)
      return Type;

    // Skip if max already reached
    if (Data.MaxPerLocation > 0 && PlacedCount.FindRef(Type) >= Data.MaxPerLocation)
      continue;

    // Skip if no rarity
    if (Data.Rarity <= 0.f)
      continue;

    WeightedList.Add({Type, Data.Rarity});
    TotalWeight += Data.Rarity;
  }

  if (WeightedList.Num() == 0)
  {
    return EAreaType::DefaultArena;
  }

  float Roll = FMath::FRandRange(0.f, TotalWeight);
  for (const auto& Entry : WeightedList)
  {
    if (Roll < Entry.Weight)
      return Entry.Type;
    Roll -= Entry.Weight;
  }

  return WeightedList.Last().Type;
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

void ULocationManager::InitArea()
{
  PlacePlayerInStartingPoint();
  OnInitAreaDelegate.Broadcast();
}

void ULocationManager::ExitArea(ECardinalDirection ExitDirection)
{
  LastExit = ExitDirection;
  CameraBoundaryActors.Empty();

  const FIntPoint& NextAreaCoordinate = UUtilityLibrary::GetCoordinateOffsetFromDirection(ExitDirection) + PlayerCoordinate;
  const FAreaData* NextAreaData = LocationLayout.Find(NextAreaCoordinate);

  GUARD(NextAreaData,, TEXT("Could not find next area at [%d,%d]!"), NextAreaCoordinate.X, NextAreaCoordinate.Y)

  LoadArea(*NextAreaData);
  
  OnExitAreaDelegate.Broadcast();
}

int32 ULocationManager::GetCurrentRegionRecommendedLevel()
{
  if (Region == ERegion::Undefined) return 1;
  
  if (!GetRegionInfo()) return 1;

  const FRegionData* RegionData = RegionInfo->GetRegionData(Region);
  if (!RegionData) return 1;

  return RegionInfo->GetRegionData(Region)->RecommendedLevel;
}

void ULocationManager::BeginPlay()
{
  Super::BeginPlay();

  if (bPCGActive)
  {
    GenerateLocation();
  }

  OnInitLocation.Broadcast();
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

FAreaData ULocationManager::GetAreaFromPoolByType(EAreaType AreaType)
{
  switch (AreaType)
  {
  case EAreaType::Entrance:
    return GetEntranceFromPool();

  case EAreaType::DefaultArena:
    return GetDefaultArenaFromPool();

  case EAreaType::SpiritArena:
    return GetSpiritArenaFromPool();

  case EAreaType::BossArena:
    return GetBossArenaFromPool();

  case EAreaType::RewardArea:
    return GetRewardAreaFromPool();

  case EAreaType::SpecialArea:
    return GetSpecialAreaFromPool();

  case EAreaType::Exit:
    return GetExitFromPool();

  default:
    return GetDefaultArenaFromPool();
  }
}

FAreaData ULocationManager::GetEntranceFromPool()
{
  FAreaData Data = GetAreaFromPool(EntrancesPool, GetRegionInfo()->GetEntrances(Region));
  Data.AreaType = EAreaType::Entrance;
  return Data;
}

FAreaData ULocationManager::GetDefaultArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(DefaultArenasPool, GetRegionInfo()->GetDefaultArenas(Region));
  Data.AreaType = EAreaType::DefaultArena;
  return Data;
}

FAreaData ULocationManager::GetSpiritArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(SpiritArenasPool, GetRegionInfo()->GetSpiritArenas(Region));
  Data.AreaType = EAreaType::SpiritArena;
  return Data;
}

FAreaData ULocationManager::GetBossArenaFromPool()
{
  FAreaData Data = GetAreaFromPool(BossArenasPool, GetRegionInfo()->GetBossArenas(Region));
  Data.AreaType = EAreaType::BossArena;
  return Data;
}

FAreaData ULocationManager::GetRewardAreaFromPool()
{
  FAreaData Data = GetAreaFromPool(RewardAreasPool, GetRegionInfo()->GetRewardAreas(Region));
  Data.AreaType = EAreaType::RewardArea;
  return Data;
}

FAreaData ULocationManager::GetSpecialAreaFromPool()
{
  FAreaData Data = GetAreaFromPool(SpecialAreasPool, GetRegionInfo()->GetSpecialAreas(Region));
  Data.AreaType = EAreaType::SpecialArea;
  return Data;
}

FAreaData ULocationManager::GetExitFromPool()
{
  FAreaData Data = GetAreaFromPool(ExitsPool, GetRegionInfo()->GetExits(Region));
  Data.AreaType = EAreaType::Exit;
  return Data;
}

void ULocationManager::HandleArenaGeneration(FAreaData& AreaData)
{
  AreaData.ArenaLevel = ArenaLevel++;

  UCombatManager* CombatManager = UAuraSystemsLibrary::GetCombatManager(GetOwner());

  GUARD(CombatManager, , TEXT("CombatManager is invalid!"))

  CombatManager->GenerateArenaCombat(AreaData);
}

void ULocationManager::LoadArea(const FAreaData& AreaData)
{
  GUARD(AreaData.World.IsNull(),, TEXT("World level asset is invalid!"))

  PrevPlayerCoordinate = PlayerCoordinate;
  PlayerCoordinate = AreaData.Coordinate;
  
  FLatentActionInfo LatentInfo;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = FName("OnAreaLoaded");
  LatentInfo.Linkage = 0;
  LatentInfo.UUID = __LINE__;

  UGameplayStatics::LoadStreamLevelBySoftObjectPtr(
    this,
    AreaData.World,
    true,
    false,
    LatentInfo
  );
}

void ULocationManager::OnAreaLoaded()
{
  InitArea();

  const FAreaData* AreaData = LocationLayout.Find(PrevPlayerCoordinate);
  if (AreaData) UnloadArea(*AreaData);
}

void ULocationManager::UnloadArea(const FAreaData& AreaData)
{
  GUARD(AreaData.World.IsNull(),, TEXT("World level asset is invalid!"))
  
  FLatentActionInfo LatentInfo;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = FName("OnAreaUnloaded");
  LatentInfo.Linkage = 0;
  LatentInfo.UUID = __LINE__;

  UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(
      this,
      AreaData.World,
      LatentInfo,
      false
  );
}

void ULocationManager::OnAreaUnloaded()
{
  
}

bool ULocationManager::IsCoordinateFree(const FIntPoint& Coordinate) const
{
  return !LocationLayout.Contains(Coordinate);
}

FIntPoint ULocationManager::GetAdjacentFreeCoordinate(const FIntPoint& Coordinate) const
{
  auto GetFreeAdj = [this](const FIntPoint& Base) -> TArray<FIntPoint>
  {
    return UUtilityLibrary::GetAdjacentCoordinates(Base)
      .FilterByPredicate([this](const FIntPoint& AdjCoordinate)
      {
        return IsCoordinateFree(AdjCoordinate);
      });
  };

  TArray<FIntPoint> FreeCoordinates = GetFreeAdj(Coordinate);

  if (FreeCoordinates.Num() == 0)
  {
    // Backtrack
    TArray<FIntPoint> PrevCoordinates;
    LocationLayout.GenerateKeyArray(PrevCoordinates);
    PrevCoordinates.Remove(Coordinate);

    Algo::RandomShuffle(PrevCoordinates);

    for (const FIntPoint& BacktrackCoordinate : PrevCoordinates)
    {
      FreeCoordinates = GetFreeAdj(BacktrackCoordinate);
      if (FreeCoordinates.Num() > 0)
        break;
    }
  }

  return FreeCoordinates[FMath::RandRange(0, FreeCoordinates.Num() - 1)];
}

void ULocationManager::ConnectAreas(FAreaData& FromArea, FAreaData& ToArea, ECardinalDirection Direction)
{
  FromArea.OpenDirections.Add(Direction);

  const ECardinalDirection OppositeDir = UUtilityLibrary::GetOppositeDirection(Direction);

  ToArea.OpenDirections.Add(OppositeDir);
}

void ULocationManager::PlaceAreaAtCoordinate(const FIntPoint& Coordinate, const FAreaData& AreaData)
{
  FAreaData NewAreaData = AreaData;
  NewAreaData.Coordinate = Coordinate;
  if (AreaData.IsArena()) HandleArenaGeneration(NewAreaData);
  
  PrevCoordinate = CurrentCoordinate;
  LocationLayout.Add(Coordinate, NewAreaData);
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
