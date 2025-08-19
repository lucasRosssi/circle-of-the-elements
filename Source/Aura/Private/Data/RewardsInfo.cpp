// Copyright Lucas Rossi


#include "Data/RewardsInfo.h"

#include "AuraGameplayTags.h"

URewardsInfo::URewardsInfo()
{
		for (const auto Reward : Rewards)
		{
			DropSum += Reward.Value.DropRateWeight;
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
	if (PropertyName == GET_MEMBER_NAME_CHECKED(FRewardInfo, DropRateWeight))
	{
		DropSum = 0.f;
		for (const auto& Pair : Rewards)
		{
			DropSum += Pair.Value.DropRateWeight;
		}
	}
}
#endif

FRewardInfo URewardsInfo::GetRewardInfo(const FGameplayTag& RewardTag)
{
	return *Rewards.Find(RewardTag);
}

FRewardInfo URewardsInfo::GetRewardInfoByElement(const FGameplayTag& ElementTag)
{
  return *Rewards.Find(*FAuraGameplayTags::Get().EssenceToAbility.FindKey(ElementTag));
}

FRewardInfo URewardsInfo::GetRandomizedReward()
{
	float TotalDropSum = DropSum;
	for (const auto Reward : Rewards)
	{
		const float RandomPick = FMath::FRandRange(0.f, TotalDropSum);

		if (RandomPick < Reward.Value.DropRateWeight) return Reward.Value;

		TotalDropSum -= RandomPick;
	}

	return FRewardInfo();
}

void URewardsInfo::FillRewardBag(
    TArray<FGameplayTag>& OutBag,
    const FGameplayTagContainer& BlockedRewards
    )
{
	for (const auto& Reward : Rewards)
	{
		for (int32 i = 0; i < Reward.Value.DropRateWeight; i++)
		{
		  if (!BlockedRewards.HasTagExact(Reward.Key))
		  {
			  OutBag.Add(Reward.Key);
		  }
		}
	}
}
