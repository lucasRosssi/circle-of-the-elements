// Copyright Lucas Rossi


#include "Game/Components/EncounterManager.h"

#include "AuraGameplayTags.h"
#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"

void UEncounterManager::SetEncounterDifficulty()
{
	const int32 EncountersCount = GetAuraGameMode()->EncountersCount;
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

void UEncounterManager::SetCurrentEncounterData()
{
	CurrentWave = 0;
	SetEncounterDifficulty();
	GetEnemySpawns();
	GetAvailableSpawners();
}

void UEncounterManager::StartEncounter()
{
	GetAuraGameMode()->EncountersCount += 1;
	
	NextWave();
}

void UEncounterManager::NextWave()
{
	if (EnemyWaves.IsEmpty())
	{
		UE_LOG(LogAura, Error, TEXT("No enemy waves to spawn!"));
		FinishEncounter();
		return;
	}
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

void UEncounterManager::FinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnFinishEncounter);
	
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SlowMotionTimerHandle,
		this,
		&UEncounterManager::PostFinishEncounter,
		TimeDilationResetDelay * UGameplayStatics::GetGlobalTimeDilation(this),
		false
		);
}

void UEncounterManager::PostFinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 1.0f);

	OnEncounterFinishedDelegate.Broadcast();
}

void UEncounterManager::GetAvailableSpawners()
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
			EnemySpawner->EnemySpawnedDelegate.AddDynamic(this, &UEncounterManager::OnEnemySpawned);
			EnemySpawner->SpawnedEnemyDeathDelegate.AddDynamic(this, &UEncounterManager::OnEnemyKilled);
		}
	}
}

void UEncounterManager::GetEnemySpawns()
{
	if (!bOverrideEnemyWaves)
	{
		EnemyWaves = GetAuraGameMode()->RegionInfo->GetRandomizedEnemyWaves(
			Region, 
			DifficultyClass,
			TotalWaves
			);
	}
}

void UEncounterManager::OnEnemySpawned(AActor* Enemy)
{
	EnemyCount += 1;
}

void UEncounterManager::OnEnemyKilled(AActor* Enemy)
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
					&UEncounterManager::NextWave,
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
