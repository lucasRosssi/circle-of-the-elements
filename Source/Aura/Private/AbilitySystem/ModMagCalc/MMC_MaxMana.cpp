// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	AttunementDef.AttributeToCapture = UAuraAttributeSet::GetAttunementAttribute();
	AttunementDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AttunementDef.bSnapshot = false;

  WillpowerDef.AttributeToCapture = UAuraAttributeSet::GetWillpowerAttribute();
  WillpowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  WillpowerDef.bSnapshot = false;

  FaithDef.AttributeToCapture = UAuraAttributeSet::GetFaithAttribute();
  FaithDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
  FaithDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(AttunementDef);
	RelevantAttributesToCapture.Add(WillpowerDef);
	RelevantAttributesToCapture.Add(FaithDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Attunement = 0;
	GetCapturedAttributeMagnitude(AttunementDef, Spec, EvaluationParams, Attunement);
	Attunement = FMath::Max(Attunement, 0.f);

  float Willpower = 0;
  GetCapturedAttributeMagnitude(WillpowerDef, Spec, EvaluationParams, Willpower);
  Willpower = FMath::Max(Willpower, 0.f);

  float Faith = 0;
  GetCapturedAttributeMagnitude(FaithDef, Spec, EvaluationParams, Faith);
  Faith = FMath::Max(Faith, 0.f);

	const int32	Level = ICombatInterface::Execute_GetCharacterLevel(
		Spec.GetContext().GetSourceObject()
	);		

	return 50.f
    + 2.f * Attunement
    + 2.f * Willpower
    + 2.f * Faith
    + 5.f * (Level - 1);
}
