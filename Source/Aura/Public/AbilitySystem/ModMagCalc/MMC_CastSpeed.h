// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_CastSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_CastSpeed : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_CastSpeed();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition AttunementDef;
  FGameplayEffectAttributeCaptureDefinition WillpowerDef;
};
