// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Parry.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_Parry::UMMC_Parry()
{
  ToughnessDef.AttributeToCapture = UAuraAttributeSet::GetToughnessAttribute();
  ToughnessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  ToughnessDef.bSnapshot = false;
  
  WillpowerDef.AttributeToCapture = UAuraAttributeSet::GetWillpowerAttribute();
  WillpowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WillpowerDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ToughnessDef);
	RelevantAttributesToCapture.Add(WillpowerDef);
}

float UMMC_Parry::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float Toughness = 0;
  GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
  Toughness = FMath::Max<float>(Toughness, 0.f);
  
  float Willpower = 0;
  GetCapturedAttributeMagnitude(WillpowerDef, Spec, EvaluationParameters, Willpower);
  Willpower = FMath::Max<float>(Willpower, 0.f);
	
	return Toughness * 0.005f
    + Willpower * 0.005f;
}
