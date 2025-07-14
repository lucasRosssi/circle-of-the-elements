// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_AttackSpeed.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_AttackSpeed::UMMC_AttackSpeed()
{
	FerocityDef.AttributeToCapture = UAuraAttributeSet::GetFerocityAttribute();
	FerocityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	FerocityDef.bSnapshot = false;

  AgilityDef.AttributeToCapture = UAuraAttributeSet::GetAgilityAttribute();
  AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AgilityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(FerocityDef);
	RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_AttackSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
  
  float Ferocity = 0;
  GetCapturedAttributeMagnitude(FerocityDef, Spec, EvaluationParameters, Ferocity);
  Ferocity = FMath::Max<float>(Ferocity, 0.f);

  float Agility = 0;
  GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
  Agility = FMath::Max<float>(Agility, 0.f);
	
	return 1
    + Ferocity * 0.0025f
    + Agility * 0.005f;
}
