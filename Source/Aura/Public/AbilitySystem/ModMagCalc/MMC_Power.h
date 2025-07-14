// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Power.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Power : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Power();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition FerocityDef;
  FGameplayEffectAttributeCaptureDefinition AgilityDef;
  FGameplayEffectAttributeCaptureDefinition ToughnessDef;
  FGameplayEffectAttributeCaptureDefinition AttunementDef;
  FGameplayEffectAttributeCaptureDefinition WillpowerDef;
  FGameplayEffectAttributeCaptureDefinition FaithDef;
};
