// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Dodge : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Dodge();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition AgilityDef;
  FGameplayEffectAttributeCaptureDefinition FaithDef;
};
