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
	URewardManager();
	
	void SetNextReward(const FGameplayTag& InRewardTag) { NextRewardTag = InRewardTag; }

	void SetGatesRewards();

	UFUNCTION(BlueprintCallable)
	void SpawnReward();

	UPROPERTY(BlueprintAssignable)
	FOnRewardTaken OnRewardTakenDelegate;

protected:
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
