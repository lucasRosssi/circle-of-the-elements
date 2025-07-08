// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CriticalDamage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_CriticalDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_CriticalDamage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition FerocityDef;
  FGameplayEffectAttributeCaptureDefinition WillpowerDef;
};
