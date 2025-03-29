// Copyright Lucas Rossi


#include "Managers/RewardManager.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/LocationReward.h"
#include "Algo/RandomShuffle.h"
#include "Aura/AuraLogChannels.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractComponent.h"
#include "Game/AuraSaveGame.h"
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
    if (GetSaveGame() && SaveGame->bJustLoaded)
    {
      RewardBag = SaveGame->RewardManager.RewardBag;
    }
    else
    {
      RewardBag.Empty();
    }
  }
}

FRewardInfo URewardManager::GetRewardInfo(const FGameplayTag& RewardTag)
{
  return UAuraSystemsLibrary::GetRewardsInfo(this)->GetRewardInfo(RewardTag);
}

void URewardManager::SpawnReward(FName AreaName)
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
      PlayerLocation.Z + Reward->GetInteractComponent_Implementation()->GetInteractAreaRadius()));
    Reward->FinishSpawning(Transform);
  }
  else
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Failed to spawn Reward: %s"),
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
  if (GetSaveGame())
  {
    SaveGame->RewardManager.BlockedRewards = BlockedRewards;
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
