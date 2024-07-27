// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PrimePower.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_PrimePower : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_PrimePower();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition PrimeAttributeDef;	
};
