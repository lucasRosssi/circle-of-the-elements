// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Dodge.h"
#include "AbilitySystem/AuraAttributeSet.h"

UMMC_Dodge::UMMC_Dodge()
{
  AgilityDef.AttributeToCapture = UAuraAttributeSet::GetAgilityAttribute();
  AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AgilityDef.bSnapshot = false;

  FaithDef.AttributeToCapture = UAuraAttributeSet::GetFaithAttribute();
  FaithDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  FaithDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AgilityDef);
	RelevantAttributesToCapture.Add(FaithDef);
}

float UMMC_Dodge::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
  
  float Agility = 0;
  GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
  Agility = FMath::Max<float>(Agility, 0.f);
  
  float Faith = 0;
  GetCapturedAttributeMagnitude(FaithDef, Spec, EvaluationParameters, Faith);
  Faith = FMath::Max<float>(Faith, 0.f);
	
	return Agility * 0.005f
    + Faith * 0.005f;
}
