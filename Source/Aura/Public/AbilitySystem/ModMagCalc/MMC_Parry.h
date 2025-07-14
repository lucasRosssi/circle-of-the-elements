// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Parry.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Parry : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Parry();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
  FGameplayEffectAttributeCaptureDefinition ToughnessDef;
  FGameplayEffectAttributeCaptureDefinition WillpowerDef;
};
