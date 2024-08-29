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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitLocation, EGatePosition, NextGatePosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEncounterFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewardTaken);

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

	void AddToXPStack(float InXP);

	UFUNCTION(BlueprintCallable)
	void LoadLevelInfo();

	void SetNextReward(const FGameplayTag& InRewardTag);

	int32 GetEnemiesLevel() const;

	FOnEncounterFinished& GetOnEncounterFinishedDelegate();
	FOnExitLocation& GetOnExitLocationDelegate();
	FOnRewardTaken& GetOnRewardTakenDelegate();
	
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location")
	TObjectPtr<ULocationManagerComponent> LocationManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location|Encounter")
	TObjectPtr<UEncounterManagerComponent> EncounterManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Location|Reward")
	TObjectPtr<URewardManagerComponent> RewardManager;

private:

};
