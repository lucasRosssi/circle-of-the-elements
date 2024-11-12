// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/Direction.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class ATile;

UCLASS()
class AURA_API ADungeonGenerator : public AActor
{
  GENERATED_BODY()

public:
  ADungeonGenerator();

  float GetTileSize() const { return TileSize; }

protected:
  virtual void BeginPlay() override;

  void SpawnFirstTile();
  UFUNCTION(BlueprintPure)
  float CalculateTileLoading();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon")
  TSubclassOf<ATile> TileClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Dungeon")
  int32 MaxTiles = 1;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon")
  float TileSize = 600.f;

private:
  TArray<FIntPoint> GetAdjacentCoordinates();
  
  TArray<FIntPoint> FindFreeAdjacentCoordinates();
  
  FVector GetTileWorldLocation(const FIntPoint& Coordinate);
  
  UPROPERTY()
  TArray<AActor*> ActorsToDestroy;
  
  int32 CurrentTile = 0;

  FIntPoint CurrentCoordinate = {0,0};
  
  TMap<FIntPoint, ATile*> DungeonMap;

  TSet<FIntPoint> SpecialTiles;

  EDirection LastExit = EDirection::North;

};
