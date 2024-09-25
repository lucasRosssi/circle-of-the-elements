// Copyright Lucas Rossi


#include "Game/Components/RewardManager.h"

#include "Actor/Level/Gate.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void URewardManager::BeginPlay()
{
  Super::BeginPlay();

  if (bOverrideReward)
  {
    OverridenRewardBag = RewardBag;
  }
}

FRewardInfo URewardManager::GetRewardInfo(const FGameplayTag& RewardTag)
{
	return GetAuraGameMode()->RewardsInfo->GetRewardInfo(RewardTag);
}

void URewardManager::SetGatesRewards()
{
	TArray<AActor*> GateActors;
	UGameplayStatics::GetAllActorsOfClass(
		GetOwner(),
		AGate::StaticClass(),
		GateActors
		);

	TArray<FGameplayTag> SelectedRewards;
	TArray<FGameplayTag> ReturningRewards;
	for (const auto GateActor : GateActors)
	{
		FGameplayTag NextReward = GetNextRewardInBag();
		
		while (SelectedRewards.Contains(NextReward))
		{
			ReturningRewards.Add(NextReward);
			NextReward = GetNextRewardInBag();
		}
		
		SelectedRewards.Add(NextReward);

		if (!ReturningRewards.IsEmpty())
		{
			for (const auto& Reward : ReturningRewards)
			{
				RewardBag.Add(Reward);
			}
		}
		
		if (AGate* Gate = Cast<AGate>(GateActor))
		{
			if (Gate->bActive)
			{
				Gate->SetGateReward(NextReward);
			}
		}
	}
}

void URewardManager::SpawnReward()
{
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetOwner(), 0);
	FTransform Transform;
	FVector RandomDirection = UKismetMathLibrary::RandomUnitVector();
	RandomDirection.Z = 0.f;
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	PlayerLocation.Z -= PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector SpawnLocation = PlayerLocation + RandomDirection * 200.f;
	Transform.SetLocation(SpawnLocation);
	
	const FRewardInfo& Info = GetNextRewardInfo();
	
	ALocationReward* Reward = GetWorld()->SpawnActorDeferred<ALocationReward>(
		Info.RewardClass,
		Transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);
	
	if (Reward)
	{
		Transform.SetLocation(FVector(
			SpawnLocation.X,
			SpawnLocation.Y,
			PlayerLocation.Z + Reward->GetInteractAreaRadius()));
		Reward->FinishSpawning(Transform);
	}
	else
	{
		UE_LOG(
			LogAura,
			Error,
			TEXT("Failed to spawn Location Reward: %s"),
			*Info.RewardClass->GetName()
			);
	}
}

FGameplayTag URewardManager::GetNextRewardInBag()
{
	if (RewardBag.IsEmpty())
	{
		FillAndShuffleRewardBag();
	}

	return RewardBag.Pop();
}

void URewardManager::FillAndShuffleRewardBag()
{
	if (bOverrideReward)
	{
		RewardBag = OverridenRewardBag;
	}
	else
	{
		GetAuraGameMode()->RewardsInfo->FillRewardBag(RewardBag);
		Algo::RandomShuffle(RewardBag);
	}
}

FRewardInfo URewardManager::GetNextRewardInfo()
{
	if (!NextRewardTag.IsValid())
	{
		NextRewardTag = GetNextRewardInBag();
	}
	
	return GetAuraGameMode()->RewardsInfo->GetRewardInfo(NextRewardTag);
}
