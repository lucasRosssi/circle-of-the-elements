// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "AuraGameplayTags.h"
#include "Game/AuraGameInstance.h"
#include "Game/Components/AbilityManager.h"
#include "Game/Components/EncounterManager.h"
#include "Game/Components/LocationManager.h"
#include "Game/Components/RewardManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"

AAuraGameModeBase::AAuraGameModeBase()
{
	AbilityManager = CreateDefaultSubobject<UAbilityManager>("Ability Manager");
	AbilityManager->SetGameMode(this);
	
	LocationManager = CreateDefaultSubobject<ULocationManager>("LocationManager");
	LocationManager->SetGameMode(this);
	
	EncounterManager = CreateDefaultSubobject<UEncounterManager>("EncounterManager");
	EncounterManager->SetGameMode(this);
	
	RewardManager = CreateDefaultSubobject<URewardManager>("RewardManager");
	RewardManager->SetGameMode(this);
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetOnEncounterFinishedDelegate().AddDynamic(RewardManager, &URewardManager::SpawnReward);
}

UAuraGameInstance* AAuraGameModeBase::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	}

	return AuraGameInstance;
}

AAuraHUD* AAuraGameModeBase::GetAuraHUD(int32 PlayerIndex)
{
	if (!AuraHUDs.Contains(PlayerIndex))
	{
		AAuraHUD* AuraHUD = Cast<AAuraHUD>(
			UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD()
			);
	
		AuraHUDs.Add(PlayerIndex, AuraHUD);
	}

	return AuraHUDs[PlayerIndex];
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
