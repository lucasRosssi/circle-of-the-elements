// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_CriticalDamage.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_CriticalDamage::UMMC_CriticalDamage()
{
	FerocityDef.AttributeToCapture = UAuraAttributeSet::GetFerocityAttribute();
	FerocityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	FerocityDef.bSnapshot = false;
  
  WillpowerDef.AttributeToCapture = UAuraAttributeSet::GetWillpowerAttribute();
  WillpowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WillpowerDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(FerocityDef);
	RelevantAttributesToCapture.Add(WillpowerDef);
}

float UMMC_CriticalDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
  
  float Ferocity = 0;
  GetCapturedAttributeMagnitude(FerocityDef, Spec, EvaluationParameters, Ferocity);
  Ferocity = FMath::Max<float>(Ferocity, 0.f);
  
  float Willpower = 0;
  GetCapturedAttributeMagnitude(WillpowerDef, Spec, EvaluationParameters, Willpower);
  Willpower = FMath::Max<float>(Willpower, 0.f);
	
	return 1.50f
    + Ferocity * 0.005f
    + Willpower * 0.0025f;
}
