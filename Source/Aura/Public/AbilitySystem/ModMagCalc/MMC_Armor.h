// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Armor.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Armor : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Armor();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition FerocityDef;
  FGameplayEffectAttributeCaptureDefinition ToughnessDef;
};
