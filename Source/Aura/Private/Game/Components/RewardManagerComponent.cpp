// Copyright Lucas Rossi


#include "Game/Components/RewardManagerComponent.h"

#include "Actor/Level/Gate.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

void URewardManagerComponent::SpawnReward()
{
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetOwner(), 0);
	FTransform Transform;
	FVector RandomDirection = UKismetMathLibrary::RandomUnitVector();
	RandomDirection.Z = 0.f;
	Transform.SetLocation(PlayerPawn->GetActorLocation() + RandomDirection * 200.f);
	
	const FRewardInfo& Info = GetNextRewardInfo();
	
	ALocationReward* Reward = GetWorld()->SpawnActorDeferred<ALocationReward>(
		Info.RewardClass,
		Transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
	
	if (Reward)	Reward->FinishSpawning(Transform);
	else UE_LOG(
		LogAura,
		Error,
		TEXT("Failed to spawn Location Reward: %s"),
		*Info.RewardClass->GetName()
		);
}
