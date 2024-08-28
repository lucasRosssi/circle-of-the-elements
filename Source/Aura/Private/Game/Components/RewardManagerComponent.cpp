// Copyright Lucas Rossi


#include "Game/Components/RewardManagerComponent.h"

#include "Actor/Level/Gate.h"
#include "Algo/RandomShuffle.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

URewardManagerComponent::URewardManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

FRewardInfo URewardManagerComponent::GetNextRewardInfo()
{
	if (NextRewardTag.IsValid())
	{
		return GetAuraGameMode()->RewardsInfo->GetRewardInfo(NextRewardTag);
	}

	return GetAuraGameMode()->RewardsInfo->GetRandomizedReward();
}

void URewardManagerComponent::FillAndShuffleRewardBag()
{
	GetAuraGameMode()->RewardsInfo->FillRewardBag(RewardBag);
	Algo::RandomShuffle(RewardBag);
}

void URewardManagerComponent::SetGatesRewards()
{
	TArray<AActor*> GateActors;
	UGameplayStatics::GetAllActorsOfClass(
		GetOwner(),
		AGate::StaticClass(),
		GateActors
		);
	
	for (const auto GateActor : GateActors)
	{
		if (AGate* Gate = Cast<AGate>(GateActor))
		{
			if (Gate->bActive)
			{
				if (RewardBag.IsEmpty())
				{
					FillAndShuffleRewardBag();
				}

				Gate->SetGateReward(RewardBag.Pop());
			}
		}
	}
}

AAuraGameModeBase* URewardManagerComponent::GetAuraGameMode()
{
	if (AuraGameMode == nullptr)
	{
		UGameplayStatics::GetGameMode(GetOwner());
	}

	return AuraGameMode;
}
