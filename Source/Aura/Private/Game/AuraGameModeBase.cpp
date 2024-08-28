// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/Gate.h"
#include "Game/AuraGameInstance.h"
#include "Game/Components/EncounterManagerComponent.h"
#include "Game/Components/LocationManagerComponent.h"
#include "Game/Components/RewardManagerComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Level/RegionInfo.h"
#include "Kismet/GameplayStatics.h"

AAuraGameModeBase::AAuraGameModeBase()
{
	LocationManager = CreateDefaultSubobject<ULocationManagerComponent>("LocationManager");
	LocationManager->SetGameMode(this);
	
	EncounterManager = CreateDefaultSubobject<UEncounterManagerComponent>("EncounterManager");
	EncounterManager->SetGameMode(this);
	
	RewardManager = CreateDefaultSubobject<URewardManagerComponent>("RewardManager");
	RewardManager->SetGameMode(this);
}

TSoftObjectPtr<UWorld> AAuraGameModeBase::GetNextLocation(
	ERegion InRegion,
	EGatePosition EntrancePosition
	)
{
	GetAuraGameInstance()->SaveHeroData();

	if (bWillExitRegion)
	{
		return nullptr;
	}
	
	TSoftObjectPtr<UWorld> Level;
	if (EncountersCount < RegionInfo->GetRegionData(InRegion)->MaxEncounters)
	{
		Level = RegionInfo->GetRandomizedRegionLocation(
		InRegion,
		EntrancePosition, 
	SelectedLevels
		);
	}
	else
	{
		Level = RegionInfo->GetBossArena(InRegion);
		bWillExitRegion = true;
	}
	
	SelectedLevels.Add(Level);
	PrevLevel = CurrentLevel;
	CurrentLevel = Level;

	return Level;
}

TSoftObjectPtr<UWorld> AAuraGameModeBase::GetInitialLocation(ERegion InRegion)
{
	const TSoftObjectPtr<UWorld> Level = RegionInfo->GetRandomizedInitialLocation(InRegion);
	
	CurrentLevel = Level;
	return Level;
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetOnEncounterFinishedDelegate().AddDynamic(RewardManager, &URewardManagerComponent::SpawnReward);
}

UAuraGameInstance* AAuraGameModeBase::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	}

	return AuraGameInstance;
}

void AAuraGameModeBase::LoadLevelInfo()
{
	PlacePlayerInStartingPoint();
	EncounterManager->SetCurrentEncounterData();
	RewardManager->SetGatesRewards();
}

void AAuraGameModeBase::PlacePlayerInStartingPoint()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		APlayerStart::StaticClass(),
		PlayerStarts
		);

	if (PlayerStarts.IsEmpty()) return;

	const APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[0]);

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	Player->SetActorTransform(
		PlayerStart->GetActorTransform(),
		false,
		nullptr,
		ETeleportType::ResetPhysics
		);
}

void AAuraGameModeBase::ExitLocation(EGatePosition NextGatePosition)
{
	OnExitLocation(NextGatePosition);
}

int32 AAuraGameModeBase::GetEnemiesLevel() const
{
	return EncounterManager->GetEnemiesLevel();
}

void AAuraGameModeBase::SetNextReward(const FGameplayTag& InRewardTag)
{
	RewardManager->SetNextReward(InRewardTag);
}

FOnEncounterFinished& AAuraGameModeBase::GetOnEncounterFinishedDelegate()
{
	return EncounterManager->OnEncounterFinishedDelegate;
}
