// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "Actor/AuraProjectile.h"
#include "Enums/ProjectileHomingMode.h"
#include "ProjectileAbility.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UProjectileAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(
		const FVector& ProjectileTargetLocation,
		const FName SocketName,
		bool bSocketInWeapon = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	FScalableFloat ProjectileCount = 1;
	
	UPROPERTY(
	  EditDefaultsOnly,
	  BlueprintReadWrite,
	  Category="Projectile",
	  meta=(ClampMin=0.f, UIMin=0.f, ClampMax=360.f, UIMax=360.f)
	  )
	float ProjectileSpread = 0.f;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Projectile",
    meta=(ClampMin=0.f, UIMin=0.f, ClampMax=90.f, UIMax=90.f)
    )
  float ProjectileAimSpread = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float PitchOverride = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing")
	bool bHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing", meta=(EditCondition="bHoming"))
	float HomingActivationDelay = 0.f;

	// Homing Targeting Mode, only available when bHoming is true
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing", meta=(EditCondition="bHoming"))
	EProjectileHomingMode HomingMode = EProjectileHomingMode::OneTarget;

private:
	void SetHomingInitialTarget(AAuraProjectile* Projectile);

	UPROPERTY()
	TArray<AActor*> Targets;

	bool bScannedForTargets = false;

	int32 ProjectileSpawnCycleCount = 0;
};
