// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class AEnemySpawner;
class AAuraEnemy;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;

USTRUCT(BlueprintType)
struct FEnemyWave
{
	GENERATED_BODY()

	TArray<TSubclassOf<AAuraEnemy>> Enemies;
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category="Status Effect Info")
	TObjectPtr<UStatusEffectInfo> StatusEffectInfo;

	UFUNCTION(BlueprintCallable)
	void StartEncounter();

	UFUNCTION(BlueprintCallable)
	void NextWave();

	UFUNCTION(BlueprintCallable)
	void FinishEncounter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Encounter")
	TArray<FEnemyWave> EnemiesPerWave;
	
private:
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);
	
	UPROPERTY()
	int32 EnemyCount = 0;

	UPROPERTY()
	int32 CurrentWave = 0;

	int32 TotalWaves;

	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;
};
