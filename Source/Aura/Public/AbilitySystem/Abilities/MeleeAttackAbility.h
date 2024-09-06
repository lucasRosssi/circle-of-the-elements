// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "MeleeAttackAbility.generated.h"

enum class EMeleeHitMode : uint8;
/**
 * 
 */
UCLASS()
class AURA_API UMeleeAttackAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ExecuteAttackScan();
	UFUNCTION(BlueprintCallable)
	void HandleWeaponCollision();
	
protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode"
		)
	EMeleeHitMode MeleeHitMode;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Box", EditConditionHides)
		)
	FVector BoxHitDimensions = FVector::ZeroVector;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Sphere", EditConditionHides)
		)
	float SphereHitRadius = 100.0f;

private:
	void DrawDebugMeleeHitShape(const FVector& AbilitySocketLocation);
	TArray<AActor*> ShapeScan(const FVector& Origin);
	void ApplyEffectToHitActor(AActor* HitActor);
	UFUNCTION()
	void OnWeaponCollision(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

};
