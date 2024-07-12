// Copyright Lucas Rossi


#include "AbilitySystem/GameplayEffects/StatusEffect.h"

UStatusEffect::UStatusEffect()
{
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;
}
