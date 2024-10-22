// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Enums/AbilityHitMode.h"
#include "Enums/TargetTeam.h"
#include "GameFramework/Actor.h"

#include "AuraProjectile.generated.h"

enum class EAbilityHitMode : uint8;
class AProjectileEffect;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FAbilityParams AbilityParams;

	UFUNCTION(BlueprintCallable)
	void ActivateHomingMode();

	void ScheduleHomingActivation(float Delay);
	void HandleAreaAbility(const AActor* EffectCauser, bool& bSuccess);
	void HandleSingleTarget(AActor* OtherActor, bool& bSuccess);
	void HandleBounceHitMode(AActor* OtherActor);
	void HandlePenetrationHitMode(AActor* OtherActor);

	TObjectPtr<UNiagaraSystem> GetMuzzleEffect() { return MuzzleEffect; }
	TObjectPtr<USoundBase> GetMuzzleSound() { return MuzzleSound; }
	USphereComponent* GetHomingSphere() { return HomingRadius; }

	ETargetTeam TargetTeam = ETargetTeam::Enemies;
	EAbilityHitMode HitMode = EAbilityHitMode::Default;
	int32 MaxHitCount = 1;
	float EffectChangePerHit = 0.f;
	float BounceRadius = 500.f;
	bool bCanRepeatTarget = false;
	UPROPERTY()
	AActor* InitialTarget = nullptr;

protected:
	virtual void BeginPlay() override;
	void OnHit(bool bDeactivateEffect = true);
	virtual void Destroyed() override;
	
	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnEnteringHomingRadius(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnHomingTargetDied(AActor* DeadActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects|GameplayCue")
	FGameplayTag GameplayCueTag;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHomingTarget(AActor* Target);

	void ApplyProjectileEffect(bool& bSuccess);
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 4.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> HomingRadius;

	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<AProjectileEffect> ProjectileNiagaraEffect;

	UPROPERTY()
	AProjectileEffect* ProjectileNiagaraEffectActor;
	
	UPROPERTY(EditAnywhere, Category="Effects|Impact")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, Category="Effects|Impact")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Effects|Impact")
	float ImpactSoundVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category="Effects|Impact")
	float ImpactSoundPitch = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Effects|Muzzle")
	TObjectPtr<UNiagaraSystem> MuzzleEffect;
	
	UPROPERTY(EditAnywhere, Category="Effects|Muzzle")
	TObjectPtr<USoundBase> MuzzleSound;

	UPROPERTY(EditAnywhere, Category="Effects|Muzzle")
	float MuzzleSoundVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category="Effects|Muzzle")
	float MuzzleSoundPitch = 1.0f;

	UPROPERTY(VisibleInstanceOnly, Category="Homing")
	AActor* HomingTarget;
	
	int32 HitCount = 0;
	UPROPERTY()
	TArray<AActor*> ActorsHit;
};

