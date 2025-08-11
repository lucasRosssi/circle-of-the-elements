// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraGameModeBase.h"
#include "CombatGameMode.generated.h"

class UMatchManager;
enum class ECharacterName : uint8;
class URewardsInfo;
class URegionInfo;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;
class FOnRewardTaken;
class FOnExitArea;
class FOnCombatFinished;
class UUpgradeManager;
class URewardManager;
class UCombatManager;
class ULocationManager;
class UAbilityManager;
/**
 * 
 */
UCLASS()
class AURA_API ACombatGameMode : public AAuraGameModeBase
{
	GENERATED_BODY()

public:
	ACombatGameMode();

  // Manager Interface
	virtual UAbilityManager* GetAbilityManager_Implementation() const override { return AbilityManager; }
	virtual ULocationManager* GetLocationManager_Implementation() const override { return LocationManager; }
	virtual UCombatManager* GetCombatManager_Implementation() const override { return CombatManager; }
  virtual URewardManager* GetRewardManager_Implementation() const override { return RewardManager; }
  virtual UUpgradeManager* GetUpgradeManager_Implementation() const override { return UpgradeManager; }
  virtual UMatchManager* GetMatchManager_Implementation() const override { return MatchManager; }
  // END Manager Interface
  
	UFUNCTION(BlueprintCallable)
	void LoadLevelInfo();

	int32 GetEnemiesLevel() const;

	FOnCombatFinished& GetOnCombatFinishedDelegate();
	FOnExitArea& GetOnExitAreaDelegate();
	FOnRewardTaken& GetOnRewardTakenDelegate();

	int32 CombatsCount = 0;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Character|Ability")
	TObjectPtr<UAbilityManager> AbilityManager;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Character|Upgrade")
  TObjectPtr<UUpgradeManager> UpgradeManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location")
	TObjectPtr<ULocationManager> LocationManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location|Combat")
	TObjectPtr<UCombatManager> CombatManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Location|Reward")
	TObjectPtr<URewardManager> RewardManager;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Match")
  TObjectPtr<UMatchManager> MatchManager;

private:

};
