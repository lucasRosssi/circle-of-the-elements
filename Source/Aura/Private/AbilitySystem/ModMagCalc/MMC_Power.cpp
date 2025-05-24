// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_Power.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_Power::UMMC_Power()
{
	StrengthDef.AttributeToCapture = UAuraAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

  DexterityDef.AttributeToCapture = UAuraAttributeSet::GetDexterityAttribute();
  DexterityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  DexterityDef.bSnapshot = false;

  ConstitutionDef.AttributeToCapture = UAuraAttributeSet::GetConstitutionAttribute();
  ConstitutionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  ConstitutionDef.bSnapshot = false;

  IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
  IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  IntelligenceDef.bSnapshot = false;
  
  WisdomDef.AttributeToCapture = UAuraAttributeSet::GetWisdomAttribute();
  WisdomDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WisdomDef.bSnapshot = false;

  CharismaDef.AttributeToCapture = UAuraAttributeSet::GetCharismaAttribute();
  CharismaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  CharismaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
	RelevantAttributesToCapture.Add(DexterityDef);
	RelevantAttributesToCapture.Add(ConstitutionDef);
	RelevantAttributesToCapture.Add(IntelligenceDef);
	RelevantAttributesToCapture.Add(WisdomDef);
	RelevantAttributesToCapture.Add(CharismaDef);
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
  
  float Strength = 0;
  GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
  Strength = FMath::Max<float>(Strength, 0.f);

  float Dexterity = 0;
  GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParameters, Dexterity);
  Dexterity = FMath::Max<float>(Dexterity, 0.f);

  float Constitution = 0;
  GetCapturedAttributeMagnitude(ConstitutionDef, Spec, EvaluationParameters, Constitution);
  Constitution = FMath::Max<float>(Constitution, 0.f);

  float Intelligence = 0;
  GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
  Intelligence = FMath::Max<float>(Intelligence, 0.f);
  
  float Wisdom = 0;
  GetCapturedAttributeMagnitude(WisdomDef, Spec, EvaluationParameters, Wisdom);
  Wisdom = FMath::Max<float>(Wisdom, 0.f);
  
  float Charisma = 0;
  GetCapturedAttributeMagnitude(CharismaDef, Spec, EvaluationParameters, Charisma);
  Charisma = FMath::Max<float>(Charisma, 0.f);
	
	return 1
    + CharacterLevel * 0.05f
    + Strength * 0.005f
    + Dexterity * 0.005f
    + Constitution * 0.005f
    + Intelligence * 0.005f
    + Wisdom * 0.005f
    + Charisma * 0.005f;
}
