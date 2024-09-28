// Copyright Lucas Rossi


#include "Managers/RewardManager.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/Gate.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractComponent.h"
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
  else
  {
    RewardBag.Empty();
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

    int32 Count = 0;
    // Add a Count to get out of the while loop in some rare situations. Most of the time the loop will finish before Count reaching 5.
    while (SelectedRewards.Contains(NextReward) && Count < 5)
    {
      ReturningRewards.Add(NextReward);
      NextReward = GetNextRewardInBag();
      Count++;
    }

    AGate* Gate = Cast<AGate>(GateActor);

    if (!Gate) continue;

    if (Count == 5 && SelectedRewards.Contains(NextReward))
    // If it was the last count and the NextReward is still one already assigned to another gate, we deactivate this gate.
    {
      Gate->DeactivateGate();
    }

    if (Gate->IsActive())
    {
      Gate->SetGateReward(NextReward);
      SelectedRewards.Add(NextReward);
    }
  }

  if (ReturningRewards.IsEmpty()) return;

  for (const auto& Reward : ReturningRewards)
  {
    RewardBag.Add(Reward);
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
      PlayerLocation.Z + Reward->GetInteractComponent()->GetInteractAreaRadius()));
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

void URewardManager::RemoveRewardFromPool(const FGameplayTag& RewardTag)
{
  RewardBag = RewardBag.FilterByPredicate([RewardTag](const FGameplayTag& Tag)
  {
    return !Tag.MatchesTagExact(RewardTag);
  });

  BlockedRewards.AddTag(RewardTag);
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
    if (
      OverridenRewardBag.Num() > 0 &&
      OverridenRewardBag[OverridenRewardBag.Num() - 1].MatchesTag(FAuraGameplayTags::Get().Resources)
    )
    {
      RewardBag = OverridenRewardBag;
    }
    else
    {
      UE_LOG(LogAura, Warning,
             TEXT("Trying to override Reward Bag with invalid data! Falling back to randomizing items."))
      GetAuraGameMode()->RewardsInfo->FillRewardBag(RewardBag, BlockedRewards);
      Algo::RandomShuffle(RewardBag);
    }
  }
  else
  {
    GetAuraGameMode()->RewardsInfo->FillRewardBag(RewardBag, BlockedRewards);
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
