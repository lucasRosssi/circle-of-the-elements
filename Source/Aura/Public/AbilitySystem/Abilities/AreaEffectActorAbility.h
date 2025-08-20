// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ActiveDamageAbility.h"
#include "AreaEffectActorAbility.generated.h"

class AAreaEffectActor;
/**
 * 
 */
UCLASS()
class AURA_API UAreaEffectActorAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
	UAreaEffectActorAbility();

	// Ability Interface overrides
	virtual int32 GetAreaEffectDurationAtLevel_Implementation(int32 Level) const override;
	virtual float GetPeriodAtLevel_Implementation(int32 Level) const override;
	virtual bool IsAreaEffectActorAbility_Implementation() const override;
	// END Ability Interface overrides
protected:
	UFUNCTION(BlueprintCallable, Category="Spawning")
	AAreaEffectActor* SpawnEffectActor(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability Defaults|Area Effect")
	TSubclassOf<AAreaEffectActor> AreaEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
	FScalableFloat AreaEffectDuration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
	bool bInstant = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
	FScalableFloat Period = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
	FScalableFloat DelayImpact = 0.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
  bool bHasRadialForce = false;

  // Offset to adjust the actor in the spawn point. To change Moveable actors spawn point, check planar constraints in their Projectile Movement Component
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Area Effect")
  FVector SpawnLocationOffset = FVector();
  
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Ability Defaults|Area Effect",
    meta=(EditCondition="bHasRadialForce")
    )
  FScalableFloat Force = 0.f;
};
