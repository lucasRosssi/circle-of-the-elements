// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Cooldown.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Cooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_Cooldown();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition CooldownSpeedDef;	
};
