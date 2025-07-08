// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MovementSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MovementSpeed();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition AgilityDef;
  FGameplayEffectAttributeCaptureDefinition AttunementDef;
};
