// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	FerocityDef.AttributeToCapture = UAuraAttributeSet::GetFerocityAttribute();
	FerocityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	FerocityDef.bSnapshot = false;

  AgilityDef.AttributeToCapture = UAuraAttributeSet::GetAgilityAttribute();
  AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AgilityDef.bSnapshot = false;

  ToughnessDef.AttributeToCapture = UAuraAttributeSet::GetToughnessAttribute();
  ToughnessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  ToughnessDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(FerocityDef);
	RelevantAttributesToCapture.Add(AgilityDef);
	RelevantAttributesToCapture.Add(ToughnessDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
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

  float Toughness = 0;
  GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
  Toughness = FMath::Max<float>(Toughness, 0.f);

	const int32	CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(
		Spec.GetContext().GetSourceObject()
	);

	return 50.f
    + 3.f * Ferocity
    + 3.f * Agility
    + 3.f * Toughness
    + 10.f * (CharacterLevel - 1);
}
