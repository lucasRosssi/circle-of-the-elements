// Copyright Lucas Rossi


#include "Level/DungeonGenerator.h"

#include "Aura/AuraLogChannels.h"
#include "Level/Tile.h"

ADungeonGenerator::ADungeonGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

  SpawnFirstTile();
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

  FirstTile->TileNumber = 0;
  
  DungeonMap.Add(CurrentCoordinate, FirstTile);
  SpecialTiles.Add(CurrentCoordinate);
  
  ActorsToDestroy.Add(FirstTile);

  const int32 RandomIndex = FMath::RandRange(0, static_cast<int32>(EDirection::MAX) - 1);
  LastExit = static_cast<EDirection>(RandomIndex);
}

float ADungeonGenerator::CalculateTileLoading()
{
  return CurrentTile / MaxTiles;
}

TArray<FIntPoint> ADungeonGenerator::GetAdjacentCoordinates()
{
  const int32 X = CurrentCoordinate.X;
  const int32 Y = CurrentCoordinate.Y;

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

TArray<FIntPoint> ADungeonGenerator::FindFreeAdjacentCoordinates()
{
  TArray<FIntPoint> FreeCoordinates;

  TArray Coordinates = GetAdjacentCoordinates();

  for (const auto& Coordinate : Coordinates)
  {
    if (DungeonMap.Contains(Coordinate)) continue;

    FreeCoordinates.Add(Coordinate);
  }

  return FreeCoordinates;
}

FVector ADungeonGenerator::GetTileWorldLocation(const FIntPoint& Coordinate)
{
  const FVector TileLocationInCoordinate = FVector(Coordinate.X, Coordinate.Y, 0.0f) * TileSize;

  return GetActorLocation() + TileLocationInCoordinate;
}
