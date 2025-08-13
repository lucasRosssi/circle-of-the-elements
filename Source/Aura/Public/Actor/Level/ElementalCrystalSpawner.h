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

  UFUNCTION()
  void Enable();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
  float ZOffsetWhenEnabled = 0.f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
  float TimeToMove = 3.f;

private:
  void SpawnElementCrystal();

  FVector OriginalLocation = FVector();
};
