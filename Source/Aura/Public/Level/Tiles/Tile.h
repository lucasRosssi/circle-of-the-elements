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

USTRUCT(BlueprintType)
struct FTileSpace
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FIntPoint StartingPoint = FIntPoint(0, 0);
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FIntPoint EndingPoint = FIntPoint(0, 0);

  FIntPoint GetSize() const
  {
    return EndingPoint - StartingPoint + 1;
  }
};

UCLASS()
class AURA_API ATile : public AActor
{
  GENERATED_BODY()

public:
  ATile();

  virtual void SetExitAvailable(EDirection WorldDirection, bool bAvailable);
  
  void SetTileNumber(int32 Number);
  void SetFacingDirection(EDirection Direction);

  EDirection GetRelativeDirection(EDirection WorldDirection) const;
  FTileSpace GetTileSpace() const { return TileSpace; }

#if WITH_EDITOR
  bool bDebug = false;
#endif
protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<USceneComponent> Root;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile|Exit")
  TMap<EDirection, bool> AvailableExits;

  // Tile space in relative coordinates
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  FTileSpace TileSpace = FTileSpace();

  EDirection FacingDirection = EDirection::North;

#if WITH_EDITORONLY_DATA
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UTextRenderComponent> TileNumberTextComponent;
#endif

private:
  int32 TileNumber = 0;
  
};
