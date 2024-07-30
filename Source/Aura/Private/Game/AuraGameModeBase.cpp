// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Kismet/GameplayStatics.h"

void AAuraGameModeBase::StartEncounter()
{
	NextWave();
}

void AAuraGameModeBase::NextWave()
{
	if (EnemySpawners.Num() < EnemiesPerWave.Num())
	{
		UE_LOG(LogAura, Error, TEXT("Game Mode is trying to spawn more enemies this wave than there is available spawners"));
		return;
	}

	CurrentWave += 1;
	
	for (const auto Spawner : EnemySpawners)
	{
		Spawner->PreSpawn();
	}
}

void AAuraGameModeBase::FinishEncounter()
{
	UE_LOG(LogAura, Warning, TEXT("Encounter finished!"));

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
		if (CurrentWave == TotalWaves - 1)
		{
			FinishEncounter();
		}
		else
		{
			NextWave();
		}
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		AEnemySpawner::StaticClass(),
		Spawners
		);

	TArray<FEnemyWave> Waves = EnemiesPerWave;
	for (const auto Spawner : Spawners)
	{
		if (AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(Spawner))
		{
			EnemySpawners.Add(EnemySpawner);
			EnemySpawner->EnemySpawnedDelegate.AddDynamic(this, &AAuraGameModeBase::OnEnemySpawned);
			EnemySpawner->SpawnedEnemyDeathDelegate.AddDynamic(this, &AAuraGameModeBase::OnEnemyKilled);

			for (auto& Wave : Waves)
			{
				const int32 Index = FMath::RandRange(0, Wave.Enemies.Num() - 1);
				if (Wave.Enemies.IsValidIndex(Index))
				{
					EnemySpawner->AddEnemyClassToQueue(Wave.Enemies[Index]);
					Wave.Enemies.RemoveAt(Index);
				}
			}
		}
	}

	EnemiesPerWave.RemoveAt(0);
}
