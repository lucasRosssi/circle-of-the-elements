// Copyright Lucas Rossi


#include "Level/RewardsInfo.h"

URewardsInfo::URewardsInfo()
{
		for (const auto Reward : Rewards)
		{
			DropSum += Reward.Value.DropWeight;
		}
}

#if WITH_EDITOR
void URewardsInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that was changed
	const FName PropertyName = PropertyChangedEvent.Property ?
		PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Check if the changed property is DropWeight within the DropMap objects
	if (PropertyName == GET_MEMBER_NAME_CHECKED(FRewardInfo, DropWeight))
	{
		DropSum = 0.f;
		for (const auto& Pair : Rewards)
		{
			DropSum += Pair.Value.DropWeight;
		}
	}
}
#endif

FRewardInfo URewardsInfo::GetRewardInfo(const FGameplayTag& RewardTag)
{
	return *Rewards.Find(RewardTag);
}

FRewardInfo URewardsInfo::GetRandomizedReward()
{
	// float DropSum = 0.f;
	// for (const auto Reward : Rewards)
	// {
	// 	DropSum += Reward.Value.DropWeight;
	// }

	float TotalDropSum = DropSum;
	for (const auto Reward : Rewards)
	{
		const float RandomPick = FMath::FRandRange(0.f, TotalDropSum);

		if (RandomPick < Reward.Value.DropWeight) return Reward.Value;

		TotalDropSum -= RandomPick;
	}

	return FRewardInfo();
}
