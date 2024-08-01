// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Level/RegionInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AAuraGameModeBase::GoToLocation(TSoftObjectPtr<UWorld> Level)
{
	GetAuraGameInstance()->SaveHeroData();
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
}

void AAuraGameModeBase::StartEncounter()
{
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

	EnemyWaves.RemoveAt(0);
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

	for (const auto Player : Players)
	{
		IPlayerInterface::SafeExec_AddToXP(Player, StackedXP);
	}
	StackedXP = 0.f;
	
	OnEncounterFinishedDelegate.Broadcast();
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
	StackedXP += InXP;
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!bOverrideEnemyWaves)
	{
		EnemyWaves = EncounterInfo->GetRandomizedEnemyWaves(
			Region, 
			DifficultyClass,
			TotalWaves
			);
	}
	
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
