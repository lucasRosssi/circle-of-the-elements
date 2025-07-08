// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Power.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_Power::UMMC_Power()
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

  AttunementDef.AttributeToCapture = UAuraAttributeSet::GetAttunementAttribute();
  AttunementDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  AttunementDef.bSnapshot = false;
  
  WillpowerDef.AttributeToCapture = UAuraAttributeSet::GetWillpowerAttribute();
  WillpowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WillpowerDef.bSnapshot = false;

  FaithDef.AttributeToCapture = UAuraAttributeSet::GetFaithAttribute();
  FaithDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  FaithDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(FerocityDef);
	RelevantAttributesToCapture.Add(AgilityDef);
	RelevantAttributesToCapture.Add(ToughnessDef);
	RelevantAttributesToCapture.Add(AttunementDef);
	RelevantAttributesToCapture.Add(WillpowerDef);
	RelevantAttributesToCapture.Add(FaithDef);
}

float UMMC_Power::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

  const int32	CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(
    Spec.GetContext().GetSourceObject()
  );
  
  float Ferocity = 0;
  GetCapturedAttributeMagnitude(FerocityDef, Spec, EvaluationParameters, Ferocity);
  Ferocity = FMath::Max<float>(Ferocity, 0.f);

  float Agility = 0;
  GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
  Agility = FMath::Max<float>(Agility, 0.f);

  float Toughness = 0;
  GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
  Toughness = FMath::Max<float>(Toughness, 0.f);

  float Attunement = 0;
  GetCapturedAttributeMagnitude(AttunementDef, Spec, EvaluationParameters, Attunement);
  Attunement = FMath::Max<float>(Attunement, 0.f);
  
  float Willpower = 0;
  GetCapturedAttributeMagnitude(WillpowerDef, Spec, EvaluationParameters, Willpower);
  Willpower = FMath::Max<float>(Willpower, 0.f);
  
  float Faith = 0;
  GetCapturedAttributeMagnitude(FaithDef, Spec, EvaluationParameters, Faith);
  Faith = FMath::Max<float>(Faith, 0.f);
	
	return 1
    + CharacterLevel * 0.05f
    + Ferocity * 0.005f
    + Agility * 0.005f
    + Toughness * 0.005f
    + Attunement * 0.005f
    + Willpower * 0.005f
    + Faith * 0.005f;
}
