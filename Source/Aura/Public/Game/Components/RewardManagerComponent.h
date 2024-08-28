// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "GameplayTagContainer.h"
#include "Level/RewardsInfo.h"
#include "RewardManagerComponent.generated.h"


class AAuraGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API URewardManagerComponent : public UAuraSystemComponent
{
	GENERATED_BODY()

public:	
	void SetNextReward(const FGameplayTag& InRewardTag) { NextRewardTag = InRewardTag; }

	void SetGatesRewards();

	UFUNCTION(BlueprintCallable)
	void SpawnReward();

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
	void FillAndShuffleRewardBag();
	
	FRewardInfo GetNextRewardInfo();
	
};
