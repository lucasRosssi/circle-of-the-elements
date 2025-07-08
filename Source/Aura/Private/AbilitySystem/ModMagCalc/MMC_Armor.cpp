// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Armor.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_Armor::UMMC_Armor()
{
	FerocityDef.AttributeToCapture = UAuraAttributeSet::GetFerocityAttribute();
	FerocityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	FerocityDef.bSnapshot = false;

  ToughnessDef.AttributeToCapture = UAuraAttributeSet::GetToughnessAttribute();
  ToughnessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  ToughnessDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(FerocityDef);
	RelevantAttributesToCapture.Add(ToughnessDef);
}

float UMMC_Armor::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
  
  float Ferocity = 0;
  GetCapturedAttributeMagnitude(FerocityDef, Spec, EvaluationParameters, Ferocity);
  Ferocity = FMath::Max<float>(Ferocity, 0.f);

  float Toughness = 0;
  GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
  Toughness = FMath::Max<float>(Toughness, 0.f);
	
	return Ferocity * 0.0025f
    + Toughness * 0.005f;
}
