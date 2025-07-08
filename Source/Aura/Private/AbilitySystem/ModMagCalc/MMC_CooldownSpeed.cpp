// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_CooldownSpeed.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_CooldownSpeed::UMMC_CooldownSpeed()
{
  AttunementDef.AttributeToCapture = UAuraAttributeSet::GetAttunementAttribute();
  AttunementDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AttunementDef.bSnapshot = false;

  FaithDef.AttributeToCapture = UAuraAttributeSet::GetFaithAttribute();
  FaithDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  FaithDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AttunementDef);
	RelevantAttributesToCapture.Add(FaithDef);
}

float UMMC_CooldownSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float Attunement = 0;
  GetCapturedAttributeMagnitude(AttunementDef, Spec, EvaluationParameters, Attunement);
  Attunement = FMath::Max<float>(Attunement, 0.f);
  
  float Faith = 0;
  GetCapturedAttributeMagnitude(FaithDef, Spec, EvaluationParameters, Faith);
  Faith = FMath::Max<float>(Faith, 0.f);
	
	return 1.f
    + Attunement * 0.0075f
    + Faith * 0.015f;
}
