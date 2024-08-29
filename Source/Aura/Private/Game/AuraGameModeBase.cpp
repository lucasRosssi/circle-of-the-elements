// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "AuraGameplayTags.h"
#include "Game/Components/EncounterManagerComponent.h"
#include "Game/Components/LocationManagerComponent.h"
#include "Game/Components/RewardManagerComponent.h"

AAuraGameModeBase::AAuraGameModeBase()
{
	LocationManager = CreateDefaultSubobject<ULocationManagerComponent>("LocationManager");
	LocationManager->SetGameMode(this);
	
	EncounterManager = CreateDefaultSubobject<UEncounterManagerComponent>("EncounterManager");
	EncounterManager->SetGameMode(this);
	
	RewardManager = CreateDefaultSubobject<URewardManagerComponent>("RewardManager");
	RewardManager->SetGameMode(this);
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetOnEncounterFinishedDelegate().AddDynamic(RewardManager, &URewardManagerComponent::SpawnReward);
}

void AAuraGameModeBase::LoadLevelInfo()
{
	LocationManager->PlacePlayerInStartingPoint();
	EncounterManager->SetCurrentEncounterData();
	RewardManager->SetGatesRewards();
}

int32 AAuraGameModeBase::GetEnemiesLevel() const
{
	return EncounterManager->GetEnemiesLevel();
}

void AAuraGameModeBase::SetNextReward(const FGameplayTag& InRewardTag)
{
	RewardManager->SetNextReward(InRewardTag);
}

FOnExitLocation& AAuraGameModeBase::GetOnExitLocationDelegate()
{
	return LocationManager->OnExitLocationDelegate;
}

FOnEncounterFinished& AAuraGameModeBase::GetOnEncounterFinishedDelegate()
{
	return EncounterManager->OnEncounterFinishedDelegate;
}

FOnRewardTaken& AAuraGameModeBase::GetOnRewardTakenDelegate()
{
	return RewardManager->OnRewardTakenDelegate;
}
