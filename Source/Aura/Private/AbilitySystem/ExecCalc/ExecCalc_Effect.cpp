// Copyright Lucas Rossi


#include "AbilitySystem/ExecCalc/ExecCalc_Effect.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UExecCalc_Effect::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	FGameplayTag EffectType;
	TArray<FGameplayTag> SpecTags;
	Spec.GetDynamicAssetTags().GetGameplayTagArray(SpecTags);
	for (auto Tag : SpecTags)
	{
		if (Tag.MatchesTag(Tags.StatusEffects_Debuff) /* || Tag.MatchesTag(Tags.Effects_Buff) */)
		{
			EffectType = Tag;
			break;
		}
	}
	
	const float EffectValue = Spec.GetSetByCallerMagnitude(EffectType);
	const float EffectDuration = Spec.GetSetByCallerMagnitude(Tags.StatusEffects_Duration);

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	UAuraAbilitySystemLibrary::SetIsApplyingEffect(ContextHandle, true);
	UAuraAbilitySystemLibrary::SetEffectType(ContextHandle, EffectType);
	UAuraAbilitySystemLibrary::SetEffectValue(ContextHandle, EffectValue);
	UAuraAbilitySystemLibrary::SetEffectDuration(ContextHandle, EffectDuration);
}
