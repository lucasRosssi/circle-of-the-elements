// Copyright Lucas Rossi


#include "Managers/RewardManager.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Components/InteractComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/AuraSystemsLibrary.h"

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
  return UAuraSystemsLibrary::GetRewardsInfo(this)->GetRewardInfo(RewardTag);
}

ALocationReward* URewardManager::SpawnReward(FVector Location)
{
  FTransform Transform;
  Transform.SetLocation(Location);

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
      Location.X,
      Location.Y,
      Location.Z + Reward->GetInteractComponent_Implementation()->GetInteractAreaRadius()));
    Reward->FinishSpawning(Transform);
    return Reward;
  }
  
  UE_LOG(
    LogAura,
    Error,
    TEXT("Failed to spawn Reward: %s"),
    *Info.RewardClass->GetName()
  );

  return nullptr;
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
      UAuraSystemsLibrary::GetRewardsInfo(this)->FillRewardBag(RewardBag, BlockedRewards);
      Algo::RandomShuffle(RewardBag);
    }
  }
  else
  {
    UAuraSystemsLibrary::GetRewardsInfo(this)->FillRewardBag(RewardBag, BlockedRewards);
    Algo::RandomShuffle(RewardBag);
  }
}

FRewardInfo URewardManager::GetNextRewardInfo()
{
  return UAuraSystemsLibrary::GetRewardsInfo(this)->GetRewardInfo(GetNextRewardInBag());
}
