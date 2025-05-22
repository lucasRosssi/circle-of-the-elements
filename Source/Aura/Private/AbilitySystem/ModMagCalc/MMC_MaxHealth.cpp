// Copyright Lucas Rossi


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	ConstitutionDef.AttributeToCapture = UAuraAttributeSet::GetConstitutionAttribute();
	ConstitutionDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ConstitutionDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ConstitutionDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Constitution = 0;
	GetCapturedAttributeMagnitude(ConstitutionDef, Spec, EvaluationParameters, Constitution);
	Constitution = FMath::Max<float>(Constitution, 0.f);

	const int32	CharacterLevel = ICombatInterface::Execute_GetCharacterLevel(
		Spec.GetContext().GetSourceObject()
	);

	return 50.f + 5.f * (Constitution - 10.f) + 15.f * (CharacterLevel - 1);
}
