// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ActiveDamageAbility.h"
#include "BeamAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBeamAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Target", HidePin="Target"))
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintPure, meta=(DefaultToSelf="Target", HidePin="Target"))
	USceneComponent* GetComponentToAttachGameplayCue();

	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Target", HidePin="Target"))
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	virtual float GetBeamTickPeriod_Implementation() const override { return BeamTickPeriod; }
  float GetBeamTraceRadius() const;
  UFUNCTION(BlueprintPure)
  float GetBeamTickPeriod() const;
  float GetBeamRange() const;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Beam")
	USceneComponent* ComponentToAttachGameplayCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Beam")
	float BeamTraceRadius = 10.f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalBeamTraceRadius = 0.f;

	// Time between each damage tick
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Beam")
	float BeamTickPeriod = 0.2f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalBeamTickPeriod = 0.f;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Beam")
  FScalableFloat BeamRange = 800.f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalBeamRange = 0.f;
};
