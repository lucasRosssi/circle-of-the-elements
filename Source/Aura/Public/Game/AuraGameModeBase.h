// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "Level/RewardsInfo.h"
#include "AuraGameModeBase.generated.h"

class UUIManager;
class AAuraHUD;
enum class ECharacterName : uint8;
class UAbilityManager;
class ULocationManager;
class UEncounterManager;
class URewardManager;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitLocation, EGatePosition, GatePosition);
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

	UAbilityManager* GetAbilityManager() const { return AbilityManager; }
	ULocationManager* GetLocationManager() const { return LocationManager; }
	UEncounterManager* GetEncounterManager() const { return EncounterManager; }
	URewardManager* GetRewardManager() const { return RewardManager; }

	void AddToXPStack(float InXP);

	UFUNCTION(BlueprintCallable)
	void LoadLevelInfo();

	void SetNextReward(const FGameplayTag& InRewardTag);

	int32 GetEnemiesLevel() const;
	
	UAuraGameInstance* GetAuraGameInstance();
	AAuraHUD* GetAuraHUD(int32 PlayerIndex);

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

	UPROPERTY(EditDefaultsOnly, Category="Player")
	ECharacterName CurrentCharacterName;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Ability")
	TObjectPtr<UAbilityManager> AbilityManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location")
	TObjectPtr<ULocationManager> LocationManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location|Encounter")
	TObjectPtr<UEncounterManager> EncounterManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location|Reward")
	TObjectPtr<URewardManager> RewardManager;

private:
	TMap<int32, AAuraHUD*> AuraHUDs;
};
