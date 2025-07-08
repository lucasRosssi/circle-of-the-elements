// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_CriticalRate.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_CriticalRate::UMMC_CriticalRate()
{
  ToughnessDef.AttributeToCapture = UAuraAttributeSet::GetToughnessAttribute();
  ToughnessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  ToughnessDef.bSnapshot = false;

  FaithDef.AttributeToCapture = UAuraAttributeSet::GetFaithAttribute();
  FaithDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  FaithDef.bSnapshot = false;
  
	RelevantAttributesToCapture.Add(ToughnessDef);
	RelevantAttributesToCapture.Add(FaithDef);
}

float UMMC_CriticalRate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float Toughness = 0;
  GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
  Toughness = FMath::Max<float>(Toughness, 0.f);
  
  float Faith = 0;
  GetCapturedAttributeMagnitude(FaithDef, Spec, EvaluationParameters, Faith);
  Faith = FMath::Max<float>(Faith, 0.f);
	
	return 0.01f
    + Toughness * 0.005f
    + Faith * 0.005f;
}
