// Copyright Lucas Rossi


#include "Managers/CombatManager.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"
#include "Utils/AuraSystemsLibrary.h"

void UCombatManager::SetCurrentCombatData()
{
  CurrentWave = 0;
  
  GetEnemyWaves();
  GetAvailableSpawners();
}

void UCombatManager::StartCombat(FName AreaName)
{
  OnCombatStartedDelegate.Broadcast(AreaName);
  CurrentAreaName = AreaName;
  SetCurrentCombatData();
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
  EnemyCount = EnemyWaves[0].Enemies.Num();

  TArray<AEnemySpawner*> Spawners = EnemySpawners;
  for (const auto Enemy : EnemyWaves[0].Enemies)
  {
    const int32 SpawnerIndex = FMath::RandRange(0, Spawners.Num() - 1);
    if (Spawners.IsValidIndex(SpawnerIndex))
    {
      Spawners[SpawnerIndex]->AddEnemyClassToQueue(Enemy.EnemyClass);
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

  OnCombatFinishedDelegate.Broadcast(CurrentAreaName);

  // TArray<AActor*> Projectiles;
  // UGameplayStatics::GetAllActorsOfClass(
  //   this,
  //   AAuraProjectile::StaticClass(),
  //   Projectiles
  // );
  //
  // TArray<AActor*> AreaEffectActors;
  // UGameplayStatics::GetAllActorsOfClass(
  //   this,
  //   AAreaEffectActor::StaticClass(),
  //   AreaEffectActors
  // );
  //
  // for (const auto Projectile : Projectiles)
  // {
  //   if (IsValid(Projectile)) Projectile->Destroy();
  // }
  //
  // for (const auto Actor : AreaEffectActors)
  // {
  //   if (IsValid(Actor)) Actor->Destroy();
  // }
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
      if (EnemySpawner->GetAreaName() != CurrentAreaName) continue;

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
    EnemyWaves = UAuraSystemsLibrary::GetRegionInfo(this)->GetEnemyWaves(
      CurrentAreaName,
      LocationName,
      Region
    );
  }

  TotalWaves = EnemyWaves.Num();
}

void UCombatManager::OnEnemySpawned(AActor* Enemy)
{
  // EnemyCount += 1;
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
