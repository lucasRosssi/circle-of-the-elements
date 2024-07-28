// Copyright Lucas Rossi


#include "..\..\..\Public\AbilitySystem\ModMagCalc\MMC_PrimePower.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_PrimePower::UMMC_PrimePower()
{
	PrimeAttributeDef.AttributeToCapture = UAuraAttributeSet::GetPrimeAttribute();
	PrimeAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	PrimeAttributeDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(PrimeAttributeDef);
}

float UMMC_PrimePower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float PrimeAttributeValue = 0.f;
	GetCapturedAttributeMagnitude(PrimeAttributeDef, Spec, EvaluationParameters, PrimeAttributeValue);
	
	return (PrimeAttributeValue - 10.f) * 0.05f;
}
