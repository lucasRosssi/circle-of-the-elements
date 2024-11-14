// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/Direction.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

class APlayerStart;
class ATile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGeneratingTiles, float, CurrentProgress);

UCLASS()
class AURA_API ADungeonGenerator : public AActor
{
  GENERATED_BODY()

public:
  ADungeonGenerator();

  float GetTileSize() const { return TileSize; }

  void BuildDungeon();

  UFUNCTION(BlueprintPure)
  bool GetIsFinished() const { return bFinished; }

protected:
  virtual void BeginPlay() override;

  void SpawnFirstTile();
  UFUNCTION(BlueprintPure)
  float CalculateTileLoading();

  UPROPERTY(BlueprintAssignable, Category = Dungeon)
  FGeneratingTiles GeneratingTilesDelegate;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon")
  TSubclassOf<ATile> TileClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Dungeon")
  int32 MaxTiles = 1;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon")
  float TileSize = 800.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon")
  float BacktrackChanceIncrease = 0.005f;

#if WITH_EDITORONLY_DATA
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dungeon|Debug")
  bool bDebugEnabled = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Dungeon|Debug",
    meta=(EditCondition="bDebugEnabled")
    )
  float SpawnDelay = 0.1f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Dungeon|Debug",
    meta=(EditCondition="bDebugEnabled")
    )
  bool bShowArrows = true;
#endif
  
private:
  // Finds a previously spawned tile that has an empty adjacent coordinate
  void Backtrack();
  
  TArray<FIntPoint> GetAdjacentCoordinates(const FIntPoint& Coordinate);
  TArray<FIntPoint> FindEmptyAdjacentCoordinates(const FIntPoint& Coordinate);
  FIntPoint GetNextEmptyCoordinate(const FIntPoint& Coordinate);
  
  FVector GetCoordinateWorldLocation(const FIntPoint& Coordinate);

  EDirection GetCoordinateDirection(const FIntPoint& StartingCoordinate, const FIntPoint& EndingCoordinate);
  EDirection GetOppositeDirection(EDirection Direction);

  void PlacePlayerInInitialPosition();
  
  UPROPERTY()
  TArray<AActor*> ActorsToDestroy;
  
  FIntPoint LastCoordinate = {0,0};
  FIntPoint CurrentCoordinate = {0,0};
  
  TMap<FIntPoint, ATile*> DungeonMap;

  TSet<FIntPoint> SpecialTiles;

  EDirection LastExit = EDirection::None;
  EDirection NextExit = EDirection::North;

  UPROPERTY()
  APlayerStart* PlayerStart = nullptr;
  
  float BacktrackChance = 0.f;

  bool bFinished = false;
};
