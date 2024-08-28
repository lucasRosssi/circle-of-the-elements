// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/EnemySpawner.h"
#include "Actor/Level/Gate.h"
#include "Actor/Level/LocationReward.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/Components/RewardManagerComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Level/RegionInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RewardsInfo.h"

AAuraGameModeBase::AAuraGameModeBase()
{
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

void AAuraGameModeBase::StartEncounter()
{
	EncountersCount += 1;
	
	NextWave();
}

void AAuraGameModeBase::NextWave()
{
	if (EnemySpawners.Num() < EnemyWaves.Num())
	{
		UE_LOG(LogAura, Error, TEXT("Game Mode is trying to spawn more enemies this wave than there is available spawners"));
		return;
	}

	CurrentWave += 1;

	TArray<AEnemySpawner*> Spawners = EnemySpawners;
	for (const auto Enemy : EnemyWaves[0].Enemies)
	{
		const int32 SpawnerIndex = FMath::RandRange(0, Spawners.Num() - 1);
		if (Spawners.IsValidIndex(SpawnerIndex))
		{
			Spawners[SpawnerIndex]->AddEnemyClassToQueue(Enemy);
			Spawners[SpawnerIndex]->PrepareSpawn();
			Spawners.RemoveAt(SpawnerIndex);
		}
	}

	if (!bOverrideEnemyWaves) EnemyWaves.RemoveAt(0);
}

void AAuraGameModeBase::FinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnFinishEncounter);
	
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SlowMotionTimerHandle,
		this,
		&AAuraGameModeBase::PostFinishEncounter,
		TimeDilationResetDelay * UGameplayStatics::GetGlobalTimeDilation(this),
		false
		);
}

void AAuraGameModeBase::OnEnemySpawned(AActor* Enemy)
{
	EnemyCount += 1;
}

void AAuraGameModeBase::OnEnemyKilled(AActor* Enemy)
{
	EnemyCount -= 1;
	if (EnemyCount <= 0)
	{
		if (CurrentWave == TotalWaves)
		{
			FinishEncounter();
		}
		else
		{
			if (WaveTransitionDelay > 0.f)
			{
				FTimerHandle WaveTransitionTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					WaveTransitionTimerHandle,
					this,
					&AAuraGameModeBase::NextWave,
					WaveTransitionDelay,
					false
					);
			}
			else
			{
				NextWave();
			}
		}
	}
}

void AAuraGameModeBase::SetCurrentLocationInfo()
{
	CurrentWave = 0;
	EnemiesLevel = FMath::Floor(EncountersCount / 2);

	if (bOverrideEnemyWaves)
	{
		TotalWaves = EnemyWaves.Num();
	}
	else
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		if (EncountersCount < 4)
		{
			TotalWaves = FMath::RandRange(1, 3);
			DifficultyClass = GameplayTags.DifficultyClass_Easy;
		}
		else if (EncountersCount < 7)
		{
			TotalWaves = FMath::RandRange(2, 4);
			DifficultyClass = GameplayTags.DifficultyClass_Normal;
		}
		else
		{
			TotalWaves = FMath::RandRange(3, 5);
			DifficultyClass = GameplayTags.DifficultyClass_Hard;
		}
	}
}

UAuraGameInstance* AAuraGameModeBase::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	}

	return AuraGameInstance;
}

void AAuraGameModeBase::PostFinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

	// for (const auto Player : Players)
	// {
	// 	IPlayerInterface::SafeExec_AddToXP(Player, StackedXP);
	// }
	// StackedXP = 0.f;

	FActorSpawnParameters SpawnParameters;
	FTransform Transform;
	Transform.SetLocation(Players[0]->GetActorLocation());
	
	const FRewardInfo& Info = RewardManager->GetNextRewardInfo();
	
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

	OnEncounterFinishedDelegate.Broadcast();
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
	StackedXP += InXP;
}

void AAuraGameModeBase::GetAvailableSpawners()
{
	EnemySpawners.Empty();
	
	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		AEnemySpawner::StaticClass(),
		Spawners
		);
	
	for (const auto Spawner : Spawners)
	{
		if (AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(Spawner))
		{
			EnemySpawners.Add(EnemySpawner);
			EnemySpawner->EnemySpawnedDelegate.AddDynamic(this, &AAuraGameModeBase::OnEnemySpawned);
			EnemySpawner->SpawnedEnemyDeathDelegate.AddDynamic(this, &AAuraGameModeBase::OnEnemyKilled);
		}
	}
}

void AAuraGameModeBase::GetEnemySpawns()
{
	if (!bOverrideEnemyWaves)
	{
		EnemyWaves = RegionInfo->GetRandomizedEnemyWaves(
			Region, 
			DifficultyClass,
			TotalWaves
			);
	}
}

void AAuraGameModeBase::LoadLevelInfo()
{
	PlacePlayerInStartingPoint();
	SetCurrentLocationInfo();
	GetEnemySpawns();
	GetAvailableSpawners();
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

void AAuraGameModeBase::SetNextReward(const FGameplayTag& InRewardTag)
{
	RewardManager->SetNextReward(InRewardTag);
}
