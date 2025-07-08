// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_CastSpeed.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_CastSpeed::UMMC_CastSpeed()
{
  AttunementDef.AttributeToCapture = UAuraAttributeSet::GetAttunementAttribute();
  AttunementDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AttunementDef.bSnapshot = false;
  
  WillpowerDef.AttributeToCapture = UAuraAttributeSet::GetWillpowerAttribute();
  WillpowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WillpowerDef.bSnapshot = false;
  
	RelevantAttributesToCapture.Add(AttunementDef);
	RelevantAttributesToCapture.Add(WillpowerDef);
}

float UMMC_CastSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float Attunement = 0;
  GetCapturedAttributeMagnitude(AttunementDef, Spec, EvaluationParameters, Attunement);
  Attunement = FMath::Max<float>(Attunement, 0.f);
  
  float Willpower = 0;
  GetCapturedAttributeMagnitude(WillpowerDef, Spec, EvaluationParameters, Willpower);
  Willpower = FMath::Max<float>(Willpower, 0.f);
	
	return 1
    + Attunement * 0.005f
    + Willpower * 0.005f;
}
