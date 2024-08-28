// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/Region.h"
#include "GameFramework/GameModeBase.h"
#include "Level/RewardsInfo.h"
#include "AuraGameModeBase.generated.h"

class ALocationReward;
class URewardsInfo;
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
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetNextLocation(ERegion InRegion, EGatePosition EntrancePosition);
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetInitialLocation(ERegion InRegion);
	
	UFUNCTION(BlueprintCallable)
	void StartEncounter();
	void NextWave();
	void FinishEncounter();
	void PostFinishEncounter();

	void AddToXPStack(float InXP);

	int32 GetEnemiesLevel() const { return EnemiesLevel; }

	UFUNCTION(BlueprintCallable)
	void GetAvailableSpawners();

	UFUNCTION(BlueprintCallable)
	void GetEnemySpawns();

	UFUNCTION(BlueprintCallable)
	void LoadLevelInfo();

	UFUNCTION(BlueprintCallable)
	void PlacePlayerInStartingPoint();

	UFUNCTION(BlueprintImplementableEvent)
	void OnExitLocation(EGatePosition NextGatePosition);
	UFUNCTION(BlueprintCallable)
	void ExitLocation(EGatePosition NextGatePosition);

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLevel() const { return CurrentLevel; }

	void SetNextReward(const FGameplayTag& InRewardTag) { NextReward = InRewardTag; }
	
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

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TObjectPtr<URewardsInfo> RewardsInfo;

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

	UPROPERTY(EditDefaultsOnly, Category="Location|Reward")
	bool bOverrideReward = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Reward",
		meta=(EditCondition="bOverrideReward", Categories="Resources")
		)
	FGameplayTag NextReward = FGameplayTag();
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Reward",
		meta=(EditCondition="bOverrideReward", Categories="Resources")
		)
	TArray<FGameplayTag> RewardBag;
	
	UPROPERTY(BlueprintReadWrite,	Category="Player")
	TArray<AActor*> Players;

private:
	UFUNCTION()
	void OnEnemySpawned(AActor* Enemy);
	UFUNCTION()
	void OnEnemyKilled(AActor* Enemy);

	void SetCurrentLocationInfo();

	FRewardInfo GetNextRewardInfo();

	void FillAndShuffleRewardBag();

	void SetGatesRewards();

	UAuraGameInstance* GetAuraGameInstance();

	UPROPERTY()
	UAuraGameInstance* AuraGameInstance = nullptr;

	FLatentActionInfo OnLoadStreamLatentActionInfo;
	
	UPROPERTY()
	int32 EnemyCount = 0;

	UPROPERTY()
	int32 CurrentWave = 0;

	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawners;

	UPROPERTY()
	float StackedXP = 0.f;

	TArray<TSoftObjectPtr<UWorld>> SelectedLevels;
	TSoftObjectPtr<UWorld> PrevLevel;
	TSoftObjectPtr<UWorld> CurrentLevel;

	int32 EncountersCount = 0;

	bool bWillExitRegion = false;
};
