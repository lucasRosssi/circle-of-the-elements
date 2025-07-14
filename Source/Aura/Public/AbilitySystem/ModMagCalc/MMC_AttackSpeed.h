// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_AttackSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_AttackSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_AttackSpeed();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition FerocityDef;
  FGameplayEffectAttributeCaptureDefinition AgilityDef;
};
