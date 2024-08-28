// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Enums/Region.h"
#include "EncounterManagerComponent.generated.h"


class AAuraGameModeBase;
class AEnemySpawner;
struct FEnemyWave;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UEncounterManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEncounterManagerComponent();

	int32 GetEnemiesLevel() const { return EnemiesLevel; }
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLocation() const { return CurrentLocation; }
	
	UFUNCTION(BlueprintCallable)
	void StartEncounter();
	void NextWave();
	void FinishEncounter();
	void PostFinishEncounter();

	// UPROPERTY(BlueprintAssignable)
	// FOnEncounterFinished OnEncounterFinishedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|Encounter")
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
	float WaveTransitionDelay = 0.f;
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
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);
	
	int32 EnemyCount = 0;
	int32 CurrentWave = 0;
	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;

	UPROPERTY()
	float StackedXP = 0.f;
	
	TSoftObjectPtr<UWorld> CurrentLocation;

	int32 EncountersCount = 0;

	AAuraGameModeBase* GetAuraGameMode();
	UPROPERTY()
	AAuraGameModeBase* AuraGameMode = nullptr;

	UPROPERTY()
	AActor* PlayerActor = nullptr;
};
