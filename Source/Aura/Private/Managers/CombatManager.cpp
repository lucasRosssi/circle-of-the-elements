// Copyright Lucas Rossi


#include "Managers/CombatManager.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Aura/AuraMacros.h"
#include "Data/EnemiesInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Data/RegionInfo.h"
#include "Utils/AuraSystemsLibrary.h"
#include "Utils/UtilityLibrary.h"

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
      Spawners[SpawnerIndex]->SetEnemyLevel(Enemy.Level);
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
      // EnemySpawner->EnemySpawnedDelegate.AddDynamic(this, &UCombatManager::OnEnemySpawned);
      EnemySpawner->SpawnedEnemyDeathDelegate.AddDynamic(this, &UCombatManager::OnEnemyKilled);
    }
  }
}

void UCombatManager::GetEnemyWaves()
{
  if (!bOverrideEnemyWaves)
  {
    const TArray<FEnemyWave>* CurrentWaves = AreasEncounters.Find(CurrentAreaName);
    EnemyWaves = CurrentWaves ? *CurrentWaves : TArray<FEnemyWave>();
  }

  TotalWaves = EnemyWaves.Num();
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

void UCombatManager::BeginPlay()
{
  Super::BeginPlay();

  LocationName = UAuraSystemsLibrary::GetCurrentLocation(GetOwner());
  SetupAreasEncounters();
}

void UCombatManager::SetupAreasEncounters()
{
  URegionInfo* RegionInfo = UAuraSystemsLibrary::GetRegionInfo(GetOwner());
  GUARD(RegionInfo != nullptr,, TEXT("RegionInfo is invalid!"))

  const FLocation& Location = RegionInfo->GetLocationData(LocationName, Region);
  GUARD(Location.IsValid(),, TEXT("Location %s is invalid!"), *LocationName.ToString())

  for (const FName& Area : Location.GetAreas())
  {
    GUARD(Location.Combats.Contains(Area),, TEXT("Area %s is invalid!"), *Area.ToString())

    const FCombat& Combat = Location.Combats[Area];

    if (Combat.Mode == ECombatMode::Defined)
    {
      AreasEncounters.Add(Area, Combat.EnemyWaves);
      continue;
    }
    
    const UEnemiesInfo* EnemiesInfo = UAuraSystemsLibrary::GetEnemiesInfo(GetOwner());
    GUARD(EnemiesInfo != nullptr, , TEXT("EnemiesInfo is invalid!"))
    
    float RemainingPoints = Combat.Data.DifficultyPoints;
    TArray<FEnemyWave> GeneratedEnemyWaves;

    while (RemainingPoints >= Combat.Data.MinWavePoints)
    {
      TArray<FEnemySpawnData> WaveData;
      float WaveBudget = FMath::RandRange(Combat.Data.MinWavePoints, Combat.Data.MaxWavePoints);

      TMap<FGameplayTag, float> EnemiesWeight = Combat.Data.EnemiesProbabilityWeight;
      if (EnemiesWeight.IsEmpty())
      {
        UE_LOG(LogAura, Error, TEXT("Enemies probability weight is empty for area %s!"), *Area.ToString())
        continue;
      }

      while (WaveBudget > 0.f)
      {
        const FGameplayTag& EnemyTag = UUtilityLibrary::PickRandomWeightedTagNormalized(EnemiesWeight);
        const FEnemyInfo& EnemyInfo = EnemiesInfo->GetEnemyInfo(EnemyTag);
        float Cost = EnemyInfo.DifficultyCost;

        if (Cost <= 0)
        {
          UE_LOG(LogAura, Error, TEXT("Enemy difficulty cost is not positive: %s"), *EnemyTag.ToString())
          EnemiesWeight.Remove(EnemyTag);
          continue;
        }

        const int32 Level = FMath::RandRange(Combat.Data.MinLevel, Combat.Data.MaxLevel);

        Cost += (Level - Combat.Data.MinLevel) * Cost * 0.1f;

        FEnemySpawnData SpawnData;
        SpawnData.EnemyClass = EnemyInfo.EnemyClass;
        SpawnData.Level = Level;
        // TODO: Add modifiers?

        WaveData.Add(SpawnData);
        WaveBudget -= Cost;
        RemainingPoints -= Cost;
      }

      GeneratedEnemyWaves.Add({WaveData});
    }

    AreasEncounters.Add(Area, GeneratedEnemyWaves);
  }
}
