// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "GameplayTagContainer.h"
#include "Game/AuraGameModeBase.h"
#include "Level/RewardsInfo.h"
#include "RewardManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API URewardManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FRewardInfo GetRewardInfo(const FGameplayTag& RewardTag);
	
	void SetNextReward(const FGameplayTag& InRewardTag) { NextRewardTag = InRewardTag; }

	void SetGatesRewards();

	UFUNCTION(BlueprintCallable)
	void SpawnReward();

  void RemoveRewardFromPool(const FGameplayTag& RewardTag);
  
	UPROPERTY(BlueprintAssignable)
	FOnRewardTaken OnRewardTakenDelegate;


protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|Reward",
    meta=(Categories="Resources")
    )
  FGameplayTagContainer BlockedRewards;
	UPROPERTY(EditDefaultsOnly, Category="Location|Reward")
	bool bOverrideReward = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Reward",
		meta=(EditCondition="bOverrideReward", Categories="Resources")
		)
	FGameplayTag NextRewardTag = FGameplayTag();
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Location|Reward",
		meta=(EditCondition="bOverrideReward", Categories="Resources")
		)
  TArray<FGameplayTag> RewardBag;

private:
	FGameplayTag GetNextRewardInBag();
	void FillAndShuffleRewardBag();
	
	FRewardInfo GetNextRewardInfo();

  TArray<FGameplayTag> OverridenRewardBag;
	
};
