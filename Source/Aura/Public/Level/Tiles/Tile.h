// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/Direction.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class ADungeonGenerator;
class ADecorator;
enum class EDirection : uint8;
class UTextRenderComponent;

UCLASS()
class AURA_API ATile : public AActor
{
  GENERATED_BODY()

public:
  ATile();

  virtual void SetExitAvailable(EDirection WorldDirection, bool bAvailable);

  void SetTileNumber(int32 Number);

  void SetFacingDirection(EDirection Direction);

  EDirection GetRelativeDirection(EDirection WorldDirection);

#if WITH_EDITOR
  bool bDebug = false;
#endif
protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<USceneComponent> Root;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile|Exit")
  TMap<EDirection, bool> AvailableExits;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  FIntPoint TileSize = FIntPoint(1, 1);

  EDirection FacingDirection = EDirection::North;

#if WITH_EDITORONLY_DATA
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UTextRenderComponent> TileNumberTextComponent;
#endif

private:
  int32 TileNumber = 0;
  
};
