// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class AURA_API ATile : public AActor
{
  GENERATED_BODY()

public:
  ATile();

protected:
  virtual void BeginPlay() override;

private:
};
