// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"

#include "AuraProjectile.generated.h"

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

protected:
	virtual void BeginPlay() override;
	void OnHit();
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

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHomingTarget(AActor* Target);
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 10.f;
	
	bool bHit;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> HomingRadius;

	UPROPERTY(EditAnywhere, Category="Trail")
	TSubclassOf<AProjectileEffect> ProjectileNiagaraEffect;

	UPROPERTY()
	AProjectileEffect* ProjectileNiagaraEffectActor;
	
	UPROPERTY(EditAnywhere, Category="Impact")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere, Category="Impact")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Impact")
	float ImpactSoundVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category="Impact")
	float ImpactSoundPitch = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Muzzle")
	TObjectPtr<UNiagaraSystem> MuzzleEffect;
	
	UPROPERTY(EditAnywhere, Category="Muzzle")
	TObjectPtr<USoundBase> MuzzleSound;

	UPROPERTY(EditAnywhere, Category="Muzzle")
	float MuzzleSoundVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category="Muzzle")
	float MuzzleSoundPitch = 1.0f;

	UPROPERTY(VisibleInstanceOnly, Category="Homing")
	AActor* HomingTarget;
	
public:
	TObjectPtr<UNiagaraSystem> GetMuzzleEffect() { return MuzzleEffect; }
	TObjectPtr<USoundBase> GetMuzzleSound() { return MuzzleSound; }
};

