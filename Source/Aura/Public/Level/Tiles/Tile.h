// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
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

  virtual void SetExitAvailable(EDirection Direction, bool bAvailable);

  void SetTileNumber(int32 Number);

#if WITH_EDITOR
  bool bDebug = false;
#endif
protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UStaticMeshComponent> TileMesh;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tile|Decorator")
  TObjectPtr<UChildActorComponent> ChildDecoratorComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile|Exit")
  TMap<EDirection, bool> AvailableExits;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  FIntPoint TileSize = FIntPoint(1, 1);

#if WITH_EDITORONLY_DATA
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
  TObjectPtr<UTextRenderComponent> TileNumberTextComponent;
#endif

private:
  AActor* GetDecorator();
  
  int32 TileNumber = 0;
  
  UPROPERTY()
  ADecorator* Decorator;
  
};
