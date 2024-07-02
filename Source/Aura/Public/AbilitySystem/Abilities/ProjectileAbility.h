// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "Actor/AuraProjectile.h"
#include "ProjectileAbility.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UProjectileAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(
		const FVector& ProjectileTargetLocation,
		const FName SocketName,
		bool bSocketInWeapon = true);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	int32 ProjectileCount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float ProjectileSpread = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	float PitchOverride = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing")
	bool bHoming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile|Homing", meta=(EditCondition="bHoming"))
	float HomingActivationDelay = 0.f;
};
