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
	void HandleAreaAbility(AActor* OtherActor, const AActor* EffectCauser, bool bSuccess);
	void HandleSingleTarget(AActor* OtherActor, bool bSuccess);
	void HandleBounceHitMode(AActor* OtherActor);
	void HandlePenetrationHitMode(AActor* OtherActor);

	TObjectPtr<UNiagaraSystem> GetMuzzleEffect() { return MuzzleEffect; }
	TObjectPtr<USoundBase> GetMuzzleSound() { return MuzzleSound; }

	ETargetTeam TargetTeam = ETargetTeam::Enemies;
	EAbilityHitMode HitMode = EAbilityHitMode::Default;
	float BounceRadius = 500.f;
	int32 MaxHitCount = 0;

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
	
	int32 HitCount = 0;
	UPROPERTY()
	TArray<AActor*> ActorsHit;
};

