// Copyright Lucas Rossi


#include "AbilitySystem/ExecCalc/ExecCalc_Heal.h"

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraHealStatics
{
  TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

  AuraHealStatics()
  {
    const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
  }
};

static const AuraHealStatics& HealStatics()
{
  static AuraHealStatics DStatics;
  return DStatics;
}

UExecCalc_Heal::UExecCalc_Heal()
{
  
}

void UExecCalc_Heal::Execute_Implementation(
  const FGameplayEffectCustomExecutionParameters& ExecutionParams,
  FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
  const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
  FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

  const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
  const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

  FAggregatorEvaluateParameters EvaluationParams;
  EvaluationParams.SourceTags = SourceTags;
  EvaluationParams.TargetTags = TargetTags;

  // Get Heal Set by Caller Magnitude
  float Heal = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Heal, false);
  
  const FGameplayModifierEvaluatedData EvaluatedData(
    UAuraAttributeSet::GetIncomingHealAttribute(),
    EGameplayModOp::Additive,
    Heal
  );
  OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
