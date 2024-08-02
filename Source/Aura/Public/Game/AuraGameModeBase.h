// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/Region.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

enum class EGatePosition : uint8;
class UAuraGameInstance;
class APostProcessVolume;
struct FEnemyWave;
class URegionInfo;
class AEnemySpawner;
class AAuraEnemy;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterFinished);

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void GoToLocation(ERegion InRegion, EGatePosition EntrancePosition);
	
	UFUNCTION(BlueprintCallable)
	void StartEncounter();

	void NextWave();

	void FinishEncounter();

	void PostFinishEncounter();

	void AddToXPStack(float InXP);

	int32 GetEnemiesLevel() const { return EnemiesLevel; }

	UPROPERTY(BlueprintAssignable)
	FOnEncounterFinished OnEncounterFinishedDelegate;
	
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UCharacterInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<UStatusEffectInfo> StatusEffectInfo;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<URegionInfo> RegionInfo;

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
		meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20)
		)
	int32 EnemiesLevel = 1;
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
	
	UPROPERTY(BlueprintReadWrite,	Category="Player")
	TArray<AActor*> Players;

private:
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);

	UAuraGameInstance* GetAuraGameInstance();

	UPROPERTY()
	UAuraGameInstance* AuraGameInstance = nullptr;
	
	UPROPERTY()
	int32 EnemyCount = 0;

	UPROPERTY()
	int32 CurrentWave = 0;

	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;

	UPROPERTY()
	float StackedXP = 0.f;
};
