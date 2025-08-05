// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalCrystalSpawner.generated.h"

UCLASS()
class AURA_API AElementalCrystalSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AElementalCrystalSpawner();

protected:
	virtual void BeginPlay() override;

private:
  void SpawnElementCrystal();

};
