// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Cooldown.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_Cooldown::UMMC_Cooldown()
{
	CooldownSpeedDef.AttributeToCapture = UAuraAttributeSet::GetCooldownSpeedAttribute();
	CooldownSpeedDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CooldownSpeedDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CooldownSpeedDef);
}

float UMMC_Cooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float CooldownSpeed = 0;
	GetCapturedAttributeMagnitude(CooldownSpeedDef, Spec, EvaluationParameters, CooldownSpeed);
	CooldownSpeed = FMath::Max<float>(CooldownSpeed, 0.5f);
	
	return 1.f / CooldownSpeed;
}
