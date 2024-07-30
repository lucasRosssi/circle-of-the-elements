// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Game/EncounterInfo.h"
#include "Kismet/GameplayStatics.h"

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
			Spawners[SpawnerIndex]->PreSpawn();
			Spawners.RemoveAt(SpawnerIndex);
		}
	}

	EnemyWaves.RemoveAt(0);
}

void AAuraGameModeBase::FinishEncounter()
{
	UE_LOG(LogAura, Warning, TEXT("Encounter finished!"));

	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnFinishEncounter);
	
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SlowMotionTimerHandle,
		this,
		&AAuraGameModeBase::ResetTimeDilation,
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
			NextWave();
		}
	}
}

void AAuraGameModeBase::ResetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);
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
