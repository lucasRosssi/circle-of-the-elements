// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_MovementSpeed.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MovementSpeed::UMMC_MovementSpeed()
{
  AgilityDef.AttributeToCapture = UAuraAttributeSet::GetAgilityAttribute();
  AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AgilityDef.bSnapshot = false;

  AttunementDef.AttributeToCapture = UAuraAttributeSet::GetAttunementAttribute();
  AttunementDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AttunementDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
	RelevantAttributesToCapture.Add(AttunementDef);
}

float UMMC_MovementSpeed::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  float Agility = 0;
  GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
  Agility = FMath::Max<float>(Agility, 0.f);

  float Attunement = 0;
  GetCapturedAttributeMagnitude(AttunementDef, Spec, EvaluationParameters, Attunement);
  Attunement = FMath::Max<float>(Attunement, 0.f);
	
	return 1
    + Agility * 0.005f
    + Attunement * 0.005f;
}
