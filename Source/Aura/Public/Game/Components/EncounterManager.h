// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "GameplayTagContainer.h"
#include "Enums/Region.h"
#include "Game/AuraGameModeBase.h"
#include "EncounterManager.generated.h"


class AEnemySpawner;
struct FEnemyWave;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UEncounterManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:	
	int32 GetEnemiesLevel() const { return EnemiesLevel; }

	void SetCurrentEncounterData();
	
	UFUNCTION(BlueprintCallable)
	void StartEncounter();

	UPROPERTY(BlueprintAssignable)
	FOnEncounterFinished OnEncounterFinishedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
	ERegion Region = ERegion::Undefined;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(Categories="DifficultyClass")
		)
	FGameplayTag DifficultyClass = FGameplayTag();
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20)
		)
	int32 EnemiesLevel = 1;
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(ClampMin=1, UIMin=1)
		)
	int32 TotalWaves = 1;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(Units="Seconds")
		)
	float WaveTransitionDelay = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|Encounter")
	float TimeDilationOnFinishEncounter = 0.2f;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(Units="Seconds")
		)
	float TimeDilationResetDelay = 1.f;
	UPROPERTY(EditDefaultsOnly, Category="Location|Encounter")
	bool bOverrideEnemyWaves = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Encounter",
		meta=(EditCondition="bOverrideEnemyWaves")
		)
	TArray<FEnemyWave> EnemyWaves;

private:
	void SetEncounterDifficulty();
	void GetAvailableSpawners();
	void GetEnemySpawns();

	void NextWave();
	void FinishEncounter();
	void PostFinishEncounter();
	
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);
	
	int32 EnemyCount = 0;
	int32 CurrentWave = 0;
	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;
};
