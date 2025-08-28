// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "Actor/AuraProjectile.h"
#include "Enums/ProjectileHomingMode.h"
#include "Enums/ProjectileMotionType.h"
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

  int32 GetProjectileCount() const;
  float GetProjectileSpread() const;
  float GetProjectileSpacing() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(
		const FVector& ProjectileTargetLocation,
		const FName SocketName,
		bool bSocketInWeapon = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
  UPROPERTY(BlueprintReadWrite)
  TSubclassOf<AAuraProjectile> OverrideProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	FScalableFloat ProjectileCount = 1;
  UPROPERTY(BlueprintReadWrite)
  int32 AdditionalProjectileCount = 0;

  // Used for spawning multiple projectiles in parallel to each other
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Projectile",
    meta=(ClampMin=0.f, UIMin=0.f, EditCondition="ProjectileSpread < 15")
    )
  float ProjectileSpacing = 0.f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalProjectileSpacing = 0.f;

  // Used for spawning multiple projectiles in an arc
	UPROPERTY(
	  EditDefaultsOnly,
	  BlueprintReadWrite,
	  Category="Projectile",
	  meta=(ClampMin=0.f, UIMin=0.f, ClampMax=360.f, UIMax=360.f)
	  )
	float ProjectileSpread = 0.f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalProjectileSpread = 0.f;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Projectile",
    meta=(ClampMin=0.f, UIMin=0.f, ClampMax=90.f, UIMax=90.f)
    )
  float ProjectileAimSpread = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float PitchOverride = 0.f;

  // Overrides the projectiles own Motion Type if setting to a non-default value
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Motion")
  EProjectileMotionType ProjectileMotionType = EProjectileMotionType::Default;
  // Whether the Directional Shift should alternate on consecutive projectiles
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Motion")
  bool bMotionDirectionAlternates = false;
  // Whether the Bezier final point will be the same as the ability Target Location
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Motion")
  bool bUseTargetLocationAsBezierFinalLocation = false;
  // Whether Yo-Yo motion will make the return towards the avatar actor
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Motion")
  bool bYoYoReturnToAvatar = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing")
	bool bHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing", meta=(EditCondition="bHoming"))
	float HomingActivationDelay = 0.f;

	// Homing Targeting Mode, only available when bHoming is true
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing", meta=(EditCondition="bHoming"))
	EProjectileHomingMode HomingMode = EProjectileHomingMode::OneTarget;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Elemental Flow")
  bool bApplyElementalFlowNiagara = false;

private:
  TArray<FTransform> HandleParallelProjectiles(
    const FVector& SocketLocation,
    const FRotator& Rotation,
    const FVector& Forward
  );
  TArray<FTransform> HandleArcProjectiles(
    const FVector& SocketLocation,
    const FVector& Forward
  );
  void SetHomingInitialTarget(AAuraProjectile* Projectile);

	UPROPERTY()
	TArray<AActor*> Targets;

	bool bScannedForTargets = false;

	int32 ProjectileSpawnCycleCount = 0;
  float ProjectileDirectionShift = 1.f;
};
