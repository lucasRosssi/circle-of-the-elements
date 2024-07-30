// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/Region.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

struct FEnemyWave;
class UEncounterInfo;
class AEnemySpawner;
class AAuraEnemy;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UCharacterInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UStatusEffectInfo> StatusEffectInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UEncounterInfo> EncounterInfo;

	UFUNCTION(BlueprintCallable)
	void StartEncounter();

	void NextWave();

	void FinishEncounter();

	void ResetTimeDilation();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Encounter")
	ERegion Region = ERegion::Undefined;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Encounter",
		meta=(Categories="DifficultyClass")
		)
	FGameplayTag DifficultyClass = FGameplayTag();
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Encounter",
		meta=(ClampMin=1, UIMin=1)
		)
	int32 TotalWaves = 1;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Encounter",
		meta=(Units="Seconds")
		)
	float WaveTransitionDelay = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Encounter")
	float TimeDilationOnFinishEncounter = 0.2f;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Encounter",
		meta=(Units="Seconds")
		)
	float TimeDilationResetDelay = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="Encounter")
	bool bOverrideEnemyWaves = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Encounter",
		meta=(EditCondition="bOverrideEnemyWaves")
		)
	TArray<FEnemyWave> EnemyWaves;

	
private:
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);
	
	UPROPERTY()
	int32 EnemyCount = 0;

	UPROPERTY()
	int32 CurrentWave = 0;

	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;
};
