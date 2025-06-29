// Copyright Lucas Rossi


#include "AbilitySystem/GameplayEffects/AuraStatusEffect.h"

UAuraStatusEffect::UAuraStatusEffect()
{
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;
}
