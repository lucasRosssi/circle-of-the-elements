// Copyright Lucas Rossi


#include "Level/DungeonGenerator.h"

#include "Aura/AuraLogChannels.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Tile.h"

ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADungeonGenerator::BuildDungeon()
{
  const float Progress = static_cast<float>(DungeonMap.Num()) / static_cast<float>(MaxTiles);
  GeneratingTilesDelegate.Broadcast(Progress);
  
  if (DungeonMap.Num() >= MaxTiles)
  {
    bFinished = true;
    return;
  }

  const float Roll = FMath::FRandRange(0.f, 1.f);
  if (Roll <= BacktrackChance)
  {
    BacktrackChance = 0.00f;
    Backtrack();
    return;
  }

  BacktrackChance += BacktrackChanceIncrease;
  
  LastCoordinate = CurrentCoordinate;
  CurrentCoordinate = GetNextEmptyCoordinate(CurrentCoordinate);

  if (CurrentCoordinate == FIntPoint::ZeroValue)
  {
    Backtrack();
    return;
  }

  const FVector Location = GetCoordinateWorldLocation(CurrentCoordinate);
  const FRotator Rotation = GetActorRotation();
  ATile* Tile = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location, &Rotation));
  if (!Tile)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Failed to spawn the Tile! Check if TileClass is set.")
    );
    return;
  }

  const EDirection SelectedExit = GetCoordinateDirection(
    LastCoordinate,
    CurrentCoordinate
    );
  LastExit = GetOppositeDirection(SelectedExit);
  
  if (DungeonMap.Contains(LastCoordinate))
  {
    DungeonMap[LastCoordinate]->SetExitAvailable(SelectedExit, true);
  }
  
  DungeonMap.Add(CurrentCoordinate, Tile);
  ActorsToDestroy.Add(Tile);

  Tile->SetTileNumber(DungeonMap.Num());
  Tile->SetExitAvailable(LastExit, true);
#if WITH_EDITOR
  if (bDebugEnabled && SpawnDelay > 0.f)
  {
    FTimerHandle DebugBuildDelay;
    GetWorld()->GetTimerManager().SetTimer(
      DebugBuildDelay,
      this,
      &ADungeonGenerator::BuildDungeon,
      SpawnDelay
    );
  }
  else
  {
    BuildDungeon(); 
  }
#else
  BuildDungeon();
#endif
}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

  SpawnFirstTile();
  PlacePlayerInInitialPosition();
  BuildDungeon();
}

void ADungeonGenerator::SpawnFirstTile()
{
  const FVector Location = GetActorLocation();
  const FRotator Rotation = GetActorRotation();
  ATile* FirstTile = Cast<ATile>(GetWorld()->SpawnActor(TileClass, &Location, &Rotation));

  if (!FirstTile)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Failed to spawn the First Tile! Check if TileClass is set.")
    );
    return;
  }

  FirstTile->SetTileNumber(1);
  
  DungeonMap.Add(CurrentCoordinate, FirstTile);
  SpecialTiles.Add(CurrentCoordinate);
  
  ActorsToDestroy.Add(FirstTile);

  const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(EDirection::MAX) - 1);
  LastExit = static_cast<EDirection>(RandomIndex);

  PlayerStart = GetWorld()->SpawnActor<APlayerStart>(Location + FVector(0.f, 0.f, 200.f), Rotation);
}

float ADungeonGenerator::CalculateTileLoading()
{
  return DungeonMap.Num() / MaxTiles;
}

void ADungeonGenerator::Backtrack()
{
  TArray<FIntPoint> DungeonCoordinates;
  DungeonMap.GetKeys(DungeonCoordinates);
  
  FIntPoint NewCoordinate = FIntPoint::ZeroValue;
  while (NewCoordinate == FIntPoint::ZeroValue && !DungeonCoordinates.IsEmpty())
  {
    const int32 RandomIndex = FMath::RandRange(0, DungeonCoordinates.Num() - 1);
    const FIntPoint RandomCoordinate = DungeonCoordinates[RandomIndex];
    
    NewCoordinate = GetNextEmptyCoordinate(RandomCoordinate);

    if (NewCoordinate != FIntPoint::ZeroValue)
    {
      CurrentCoordinate = RandomCoordinate;
      break;
    }
    
    DungeonCoordinates.RemoveAt(RandomIndex);
  }

  BuildDungeon();
}

TArray<FIntPoint> ADungeonGenerator::GetAdjacentCoordinates(const FIntPoint& Coordinate)
{
  const int32 X = Coordinate.X;
  const int32 Y = Coordinate.Y;

  const FIntPoint North = FIntPoint(X + 1, Y);
  const FIntPoint East = FIntPoint(X, Y + 1);
  const FIntPoint South = FIntPoint(X - 1, Y);
  const FIntPoint West = FIntPoint(X, Y - 1);

  TArray Coordinates = {
    North,
    East,
    South,
    West
  };

  return Coordinates;
}

TArray<FIntPoint> ADungeonGenerator::FindEmptyAdjacentCoordinates(const FIntPoint& Coordinate)
{
  TArray<FIntPoint> FreeCoordinates;

  TArray Coordinates = GetAdjacentCoordinates(Coordinate);

  for (const auto& AdjCoordinate : Coordinates)
  {
    if (DungeonMap.Contains(AdjCoordinate)) continue;

    FreeCoordinates.Add(AdjCoordinate);
  }

  return FreeCoordinates;
}

FIntPoint ADungeonGenerator::GetNextEmptyCoordinate(const FIntPoint& Coordinate)
{
  const TArray<FIntPoint> FreeCoordinates = FindEmptyAdjacentCoordinates(Coordinate);

  if (FreeCoordinates.IsEmpty()) return FIntPoint::ZeroValue;
  
  const int32 RandomIndex = FMath::RandRange(0, FreeCoordinates.Num() - 1);
  
  return FreeCoordinates[RandomIndex];
}

FVector ADungeonGenerator::GetCoordinateWorldLocation(const FIntPoint& Coordinate)
{
  const FVector TileLocationInCoordinate = FVector(Coordinate.X, Coordinate.Y, 0.0f) * TileSize;

  return GetActorLocation() + TileLocationInCoordinate;
}

EDirection ADungeonGenerator::GetCoordinateDirection(
  const FIntPoint& StartingCoordinate,
  const FIntPoint& EndingCoordinate
  )
{
  const FIntPoint Difference = EndingCoordinate - StartingCoordinate;

  if (Difference.X > 0)
  {
    return EDirection::North;
  }
  
  if (Difference.X < 0)
  {
    return EDirection::South;
  }

  if (Difference.Y > 0)
  {
    return EDirection::East;
  }

  if (Difference.Y < 0)
  {
    return EDirection::West;
  }

  return EDirection::None;
}

EDirection ADungeonGenerator::GetOppositeDirection(EDirection Direction)
{
  switch (Direction)
  {
    case EDirection::North:
      return EDirection::South;
    case EDirection::East:
      return EDirection::West;
    case EDirection::South:
      return EDirection::North;
    case EDirection::West:
      return EDirection::East;
    default:
      return EDirection::None;
  }
}

void ADungeonGenerator::PlacePlayerInInitialPosition()
{
  if (!PlayerStart) return;
  
  APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
  
  Player->SetActorLocation(
    PlayerStart->GetActorLocation(),
    false,
    nullptr,
    ETeleportType::ResetPhysics
  );
}
