// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UNiagaraSystem;
class AAuraEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, AActor*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedEnemyDeath, AActor*, Enemy);

UCLASS()
class AURA_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

  FName GetAreaName() const { return AreaName; }

	UFUNCTION(BlueprintCallable)
	void AddEnemyClassToQueue(TSubclassOf<AAuraEnemy> EnemyClass);
	void PreSpawn();

	UFUNCTION(BlueprintCallable)
	void PrepareSpawn();

	void SpawnNextEnemy();

	FOnEnemySpawned EnemySpawnedDelegate;
	FOnSpawnedEnemyDeath SpawnedEnemyDeathDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	float PreSpawnDelayMax = 0.f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<UNiagaraSystem> SpawnCircle;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
  TObjectPtr<UNiagaraSystem> SpawnBurst;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	float SpawnDelay = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	bool bOverrideEnemyLevel = false;
	UPROPERTY(
		EditAnywhere, 
		BlueprintReadWrite,
		Category="Spawning",
		meta=(EditCondition="bOverrideEnemyLevel", EditConditionHides)
		)
	int32 LevelOverride = 1;

  UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Location")
  FName AreaName;

private:
	UPROPERTY()
	TArray<TSubclassOf<AAuraEnemy>> EnemyQueue;

	UFUNCTION()
	void OnSpawnedEnemyDeath(AActor* Enemy);

	UPROPERTY()
	int32 EnemyLevel = 1;
};
