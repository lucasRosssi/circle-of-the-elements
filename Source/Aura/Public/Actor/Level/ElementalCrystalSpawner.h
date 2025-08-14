// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementalCrystalSpawner.generated.h"

class ALocationReward;

UENUM(BlueprintType)
enum class ERewardSpawnerActivationMode : uint8
{
  Movement UMETA(DisplayName = "Movement"),
  Chaos UMETA(DisplayName = "Chaos"),

  MAX UMETA(Hidden)
};


UCLASS()
class AURA_API AElementalCrystalSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AElementalCrystalSpawner();

  ALocationReward* GetCrystal() const { return Crystal; }

protected:
	virtual void BeginPlay() override;

  UFUNCTION()
  void Enable();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawner")
  TObjectPtr<USceneComponent> SceneRoot;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawner")
  TObjectPtr<UStaticMeshComponent> SpawnerMesh;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner|Activation")
  ERewardSpawnerActivationMode ActivationMode = ERewardSpawnerActivationMode::Movement;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Spawner|Activation|Movement",
    meta=(EditCondition="ActivationMode == ERewardSpawnerActivationMode::Movement", EditConditionHides)
  )
  float MovementZOffset = 0.f;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Spawner|Activation|Movement", 
    meta=(EditCondition="ActivationMode == ERewardSpawnerActivationMode::Movement", EditConditionHides)
  )
  float TimeToMove = 3.f;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Spawner|Activation|Movement", 
    meta=(EditCondition="ActivationMode == ERewardSpawnerActivationMode::Movement", EditConditionHides)
  )
  bool bEaseIn = false;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Spawner|Activation|Movement", 
    meta=(EditCondition="ActivationMode == ERewardSpawnerActivationMode::Movement", EditConditionHides)
  )
  bool bEaseOut = false;

private:
  void SpawnElementCrystal();

  FVector OriginalLocation = FVector();

  UPROPERTY()
  ALocationReward* Crystal = nullptr;
};
