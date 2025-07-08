// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CriticalRate.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_CriticalRate : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_CriticalRate();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition ToughnessDef;
  FGameplayEffectAttributeCaptureDefinition FaithDef;
};
