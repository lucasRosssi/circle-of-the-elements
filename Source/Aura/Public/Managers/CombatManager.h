// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "GameplayTagContainer.h"
#include "Data/RegionInfo.h"
#include "Enums/Region.h"
#include "CombatManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatFinished);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastEnemyKilled);

class AEnemySpawner;
struct FEnemyWave;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UCombatManager : public UAuraSystemComponent
{
  GENERATED_BODY()

public:
  void GenerateArenaCombat(const FAreaData& Arena);
  
  int32 GetEnemiesLevel() const { return EnemiesLevel; }
  AActor* GetCurrentBoss() const { return CurrentBoss.Get(); }

  void SetCurrentBoss(AActor* InBoss) { CurrentBoss = InBoss; }

  void SetCurrentCombatData();

  UFUNCTION(BlueprintCallable)
  void StartCombat();

  UFUNCTION()
  void OnEnemySpawned(AActor* Enemy);
  UFUNCTION()
  void OnEnemyKilled(AActor* Enemy);

  UPROPERTY(BlueprintAssignable)
  FOnCombatFinished OnCombatStartedDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnCombatFinished OnCombatFinishedDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnLastEnemyKilled OnLastEnemyKilledDelegate;

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  ERegion Region = ERegion::Undefined;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
  FName LocationName = NAME_None;
  
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|Combat",
    meta=(ClampMin=1, UIMin=1, ClampMax=20, UIMax=20)
  )
  int32 EnemiesLevel = 1;
  UPROPERTY(
    VisibleAnywhere,
    BlueprintReadWrite,
    Category="Location|Combat",
    meta=(ClampMin=1, UIMin=1)
  )
  int32 TotalWaves = 1;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|Combat",
    meta=(Units="Seconds")
  )
  float WaveTransitionDelay = 1.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location|Combat")
  float TimeDilationOnFinishCombat = 0.2f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|Combat",
    meta=(Units="Seconds")
  )
  float TimeDilationResetDelay = 1.f;
  UPROPERTY(EditDefaultsOnly, Category="Location|Combat")
  bool bOverrideEnemyWaves = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Location|Combat",
    meta=(EditCondition="bOverrideEnemyWaves")
  )
  TArray<FEnemyWave> EnemyWaves;

private:
  void GetAvailableSpawners();
  void GetEnemyWaves();

  void NextWave();
  void FinishCombat();
  void PostFinishCombat();

  FIntPoint CurrentArenaCoordinate;
  int32 EnemyCount = 0;
  int32 CurrentWave = 0;
  UPROPERTY()
  TArray<AEnemySpawner*> EnemySpawners;

  TMap<FIntPoint, TArray<FEnemyWave>> ArenasEncounters;

  TWeakObjectPtr<AActor> CurrentBoss;
};
