// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Enums/TargetTeam.h"
#include "AreaEffectActor.generated.h"

class USphereComponent;
class UTeamComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS()
class AURA_API AAreaEffectActor : public AActor
{
  GENERATED_BODY()

public:
  AAreaEffectActor();

  UTeamComponent* GetTeamComponent() const { return TeamComponent; }
  USphereComponent* GetRadialForceSphere() const { return RadialForceSphere; }
  void SetActorDuration(float InDuration) { LifeSpan = InDuration; }

  UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
  FAbilityParams AbilityParams;

  UPROPERTY(EditDefaultsOnly, Category="Applied Effects|Target")
  ETargetTeam TargetTeam = ETargetTeam::Both;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects|Mode")
  bool bInstant = false;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category = "Applied Effects|Mode",
    meta=(EditCondition="!bInstant")
  )
  float Period = 0.f;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category = "Applied Effects|Mode",
    meta=(Units="Seconds", ClampMin=0.f, UIMin=0.f)
  )
  float DelayImpact = 0.f;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category = "Applied Effects|Force"
  )
  bool bHasRadialForce = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category = "Applied Effects|Force"
  )
  float Force = 0.f;

protected:
  virtual void BeginPlay() override;
  virtual void BeginDestroy() override;
  virtual void Tick(float DeltaSeconds) override;
  void DeactivateAndDestroy();

  UFUNCTION(BlueprintCallable)
  void ApplyEffectToTarget(UAbilitySystemComponent* TargetASC);
  void ApplyAbilityEffect(UAbilitySystemComponent* TargetASC, bool& bSuccess);

  void ApplyAbilityEffectToActorsInArea();

  UFUNCTION(BlueprintCallable)
  void OnOverlap(AActor* TargetActor);
  UFUNCTION(BlueprintCallable)
  void OnEndOverlap(AActor* TargetActor);

  UFUNCTION()
  void OnRadialForceOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
    );
  UFUNCTION()
  void OnRadialForceEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
    );

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USphereComponent> Root;
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<USphereComponent> RadialForceSphere;
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UTeamComponent> TeamComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
  TSubclassOf<UGameplayEffect> GameplayEffectClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
  TSubclassOf<UGameplayEffect> EndOverlapGameplayEffectClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
  float ActorLevel = 1.f;

  UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
  float LifeSpan = -1.f;

  UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
  float DelayDestroy = 1.f;

  UPROPERTY(EditAnywhere, Category="Effects|Sound")
  TObjectPtr<USoundBase> SpawnSound;

  UPROPERTY(EditAnywhere, Category="Effects|Sound")
  float SpawnSoundVolume = 1.0f;

  UPROPERTY(EditAnywhere, Category="Effects|Sound")
  float SpawnSoundPitch = 1.0f;

private:
  UPROPERTY()
  TArray<UAbilitySystemComponent*> ASCsInArea;
  UPROPERTY()
  TArray<AActor*> ActorsInForceArea;

  TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

  FTimerHandle PeriodicEffectTimer;

  bool bDestroying = false;
};


