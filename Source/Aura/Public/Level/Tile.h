// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

enum class EDirection : uint8;
class UTextRenderComponent;
class UArrowComponent;

UCLASS()
class AURA_API ATile : public AActor
{
  GENERATED_BODY()

public:
  ATile();

  void SetExitAvailable(EDirection Direction, bool bAvailable);

  void SetTileNumber(int32 Number);

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> TileMesh;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh1;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh2;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh3;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> UnderWallMesh4;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> NorthArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> EastArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> SouthArrow;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UArrowComponent> WestArrow;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UTextRenderComponent> TileNumberTextComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile|Exit")
  TMap<EDirection, bool> AvailableExits;

private:
  int32 TileNumber = 0;
  
  TMap<EDirection, UArrowComponent*> Arrows;
  
};
