// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Cooldown.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_Cooldown::UMMC_Cooldown()
{
	CooldownReductionDef.AttributeToCapture = UAuraAttributeSet::GetCooldownReductionAttribute();
	CooldownReductionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownReductionDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownReductionDef);
}

float UMMC_Cooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float CooldownReduction = 0;
	GetCapturedAttributeMagnitude(CooldownReductionDef, Spec, EvaluationParameters, CooldownReduction);
	CooldownReduction = FMath::Min<float>(CooldownReduction, 100.f);
	
	return (100.f - CooldownReduction) / 100;
}

