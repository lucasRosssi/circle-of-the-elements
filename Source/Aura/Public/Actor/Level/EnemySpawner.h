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

	UFUNCTION(BlueprintCallable)
	void AddEnemyClassToQueue(TSubclassOf<AAuraEnemy> EnemyClass);
	void SpawnSystem();

	UFUNCTION(BlueprintCallable)
	void PreSpawn();

	void SpawnNextEnemy();

	FOnEnemySpawned EnemySpawnedDelegate;
	FOnSpawnedEnemyDeath SpawnedEnemyDeathDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	float PreSpawnDelayMax = 0.f;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	TObjectPtr<UNiagaraSystem> SpawnEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawning")
	float SpawnDelay = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	int32 EnemyLevel = 1;

private:
	UPROPERTY()
	TArray<TSubclassOf<AAuraEnemy>> EnemyQueue;

	UFUNCTION()
	void OnSpawnedEnemyDeath(AActor* Enemy);
};
