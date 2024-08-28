// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/Region.h"
#include "GameFramework/GameModeBase.h"
#include "Level/RewardsInfo.h"
#include "AuraGameModeBase.generated.h"

class ULocationManagerComponent;
class UEncounterManagerComponent;
class URewardManagerComponent;
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
	AAuraGameModeBase();

	ULocationManagerComponent* GetLocationManager() const { return LocationManager; }
	UEncounterManagerComponent* GetEncounterManager() const { return EncounterManager; }
	URewardManagerComponent* GetRewardManager() const { return RewardManager; }
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetNextLocation(ERegion InRegion, EGatePosition EntrancePosition);
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetInitialLocation(ERegion InRegion);

	void AddToXPStack(float InXP);

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

	void SetNextReward(const FGameplayTag& InRewardTag);

	int32 GetEnemiesLevel() const;

	FOnEncounterFinished& GetOnEncounterFinishedDelegate();
	
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

	int32 EncountersCount = 0;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Location")
	TObjectPtr<ULocationManagerComponent> LocationManager;
	UPROPERTY(VisibleAnywhere, Category="Location|Encounter")
	TObjectPtr<UEncounterManagerComponent> EncounterManager;
	UPROPERTY(VisibleAnywhere, Category="Location|Reward")
	TObjectPtr<URewardManagerComponent> RewardManager;

private:
	UAuraGameInstance* GetAuraGameInstance();

	UPROPERTY()
	UAuraGameInstance* AuraGameInstance = nullptr;

	FLatentActionInfo OnLoadStreamLatentActionInfo;

	TArray<TSoftObjectPtr<UWorld>> SelectedLevels;
	TSoftObjectPtr<UWorld> PrevLevel;
	TSoftObjectPtr<UWorld> CurrentLevel;

	bool bWillExitRegion = false;
};
