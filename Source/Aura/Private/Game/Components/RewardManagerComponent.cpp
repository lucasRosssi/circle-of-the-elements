// Copyright Lucas Rossi


#include "Game/Components/RewardManagerComponent.h"

#include "Actor/Level/Gate.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URewardManagerComponent::URewardManagerComponent()
{
	if (bOverrideReward)
	{
		OverridenRewardBag = RewardBag;
	}
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
				Gate->SetGateReward(GetNextRewardInBag());
			}
		}
	}
}

void URewardManagerComponent::SpawnReward()
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

FGameplayTag URewardManagerComponent::GetNextRewardInBag()
{
	if (RewardBag.IsEmpty())
	{
		FillAndShuffleRewardBag();
	}

	return RewardBag.Pop();
}

void URewardManagerComponent::FillAndShuffleRewardBag()
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

FRewardInfo URewardManagerComponent::GetNextRewardInfo()
{
	if (!NextRewardTag.IsValid())
	{
		NextRewardTag = GetNextRewardInBag();
	}
	
	return GetAuraGameMode()->RewardsInfo->GetRewardInfo(NextRewardTag);
}
