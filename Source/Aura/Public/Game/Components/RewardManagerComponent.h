// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Level/RewardsInfo.h"
#include "RewardManagerComponent.generated.h"


class AAuraGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API URewardManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URewardManagerComponent();

	void SetGameMode(AAuraGameModeBase* InGameMode) { AuraGameMode = InGameMode; }
	void SetNextReward(const FGameplayTag& InRewardTag) { NextRewardTag = InRewardTag; }

	FRewardInfo GetNextRewardInfo();

	void FillAndShuffleRewardBag();

	void SetGatesRewards();

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
	AAuraGameModeBase* GetAuraGameMode();
	UPROPERTY()
	AAuraGameModeBase* AuraGameMode = nullptr;
};
