// Copyright Lucas Rossi


#include "Managers/CombatManager.h"

#include "AuraGameplayTags.h"
#include "Actor/AreaEffectActor.h"
#include "Actor/AuraProjectile.h"
#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"
#include "Utils/AuraSystemsLibrary.h"

void UCombatManager::SetCombatDifficulty()
{
	EnemiesLevel = FMath::Max(1, FMath::Floor(CombatsCount / 3));

	if (bOverrideEnemyWaves)
	{
		TotalWaves = EnemyWaves.Num();
	}
	else
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		if (CombatsCount == 0)
		{
			TotalWaves = 1;
			DifficultyClass = GameplayTags.DifficultyClass_Initial;
		}
		else if (CombatsCount < 4)
		{
			TotalWaves = FMath::RandRange(2, 3);
			DifficultyClass = GameplayTags.DifficultyClass_Easy;
		}
		else if (CombatsCount < 7)
		{
			TotalWaves = 3;
			DifficultyClass = GameplayTags.DifficultyClass_Normal;
		}
		else
		{
			TotalWaves = FMath::RandRange(3, 4);
			DifficultyClass = GameplayTags.DifficultyClass_Hard;
		}
	}
}

void UCombatManager::SetCurrentCombatData()
{
	CurrentWave = 0;

  if (GetSaveGame() && SaveGame->bJustLoaded)
  {
    CombatsCount = SaveGame->CombatManager.CombatsCount;
  }
  
  SetCombatDifficulty();
	GetEnemyWaves();
	GetAvailableSpawners();
}

void UCombatManager::StartCombat()
{
	CombatsCount += 1;
  if (GetSaveGame())
  {
    SaveGame->CombatManager.CombatsCount = CombatsCount;
  }
	
	NextWave();
}

void UCombatManager::NextWave()
{
	if (EnemyWaves.IsEmpty())
	{
		UE_LOG(LogAura, Error, TEXT("No enemy waves to spawn!"));
		FinishCombat();
		return;
	}
	if (EnemySpawners.Num() < EnemyWaves[0].Enemies.Num())
	{
		UE_LOG(LogAura, Error, TEXT("Trying to spawn more enemies than there is available spawners!"));
		FinishCombat();
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

	if (!bOverrideEnemyWaves)
	{
	  EnemyWaves.RemoveAt(0);
	}
}

void UCombatManager::FinishCombat()
{
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnFinishCombat);
	
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SlowMotionTimerHandle,
		this,
		&UCombatManager::PostFinishCombat,
		TimeDilationResetDelay * UGameplayStatics::GetGlobalTimeDilation(this),
		false
		);
}

void UCombatManager::PostFinishCombat()
{
	UGameplayStatics::SetGlobalTimeDilation(GetOwner(), 1.0f);

	OnCombatFinishedDelegate.Broadcast();
  
  TArray<AActor*> Projectiles;
  UGameplayStatics::GetAllActorsOfClass(
    this,
    AAuraProjectile::StaticClass(),
    Projectiles
    );

  TArray<AActor*> AreaEffectActors;
  UGameplayStatics::GetAllActorsOfClass(
    this,
    AAreaEffectActor::StaticClass(),
    AreaEffectActors
    );

  for (const auto Projectile : Projectiles)
  {
    if (IsValid(Projectile)) Projectile->Destroy();
  }
  
  for (const auto Actor : AreaEffectActors)
  {
    if (IsValid(Actor)) Actor->Destroy();
  }
}

void UCombatManager::GetAvailableSpawners()
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
			EnemySpawner->EnemySpawnedDelegate.AddDynamic(this, &UCombatManager::OnEnemySpawned);
			EnemySpawner->SpawnedEnemyDeathDelegate.AddDynamic(this, &UCombatManager::OnEnemyKilled);
		}
	}
}

void UCombatManager::GetEnemyWaves()
{
	if (!bOverrideEnemyWaves)
	{
		EnemyWaves = UAuraSystemsLibrary::GetRegionInfo(this)->GetRandomizedEnemyWaves(
			Region, 
			DifficultyClass,
			TotalWaves
			);
	}
}

void UCombatManager::OnEnemySpawned(AActor* Enemy)
{
	EnemyCount += 1;
}

void UCombatManager::OnEnemyKilled(AActor* Enemy)
{
	EnemyCount -= 1;
	if (EnemyCount <= 0)
	{
		if (CurrentWave == TotalWaves)
		{
		  OnLastEnemyKilledDelegate.Broadcast();
			FinishCombat();
		}
		else
		{
			if (WaveTransitionDelay > 0.f)
			{
				FTimerHandle WaveTransitionTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					WaveTransitionTimerHandle,
					this,
					&UCombatManager::NextWave,
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
