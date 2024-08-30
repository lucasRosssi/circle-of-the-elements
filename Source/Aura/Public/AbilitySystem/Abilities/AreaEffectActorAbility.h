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

	virtual int32 GetAreaEffectDurationAtLevel_Implementation(int32 Level) const override;
	virtual float GetPeriodAtLevel_Implementation(int32 Level) const override;
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
};
