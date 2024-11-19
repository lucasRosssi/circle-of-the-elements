// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Level/Tiles/Tile.h"
#include "TileExit.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ATileExit : public ATile
{
	GENERATED_BODY()

public:
  ATileExit();

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tile|Exit")
  TObjectPtr<UChildActorComponent> ChildGateComponent;

private:
	
};
