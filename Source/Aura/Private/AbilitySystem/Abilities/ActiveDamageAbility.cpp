// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UActiveDamageAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle =  MakeOutgoingGameplayEffectSpec
	(DamageEffectClass, 1.f);
	for (auto Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			DamageSpecHandle,
			Pair.Key,
			ScaledDamage
		);
	}
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
	);
}

float UActiveDamageAbility::GetDamageAtLevel(
	int32 Level,
	FGameplayTag DamageTypeTag
)
{
	if (const FScalableFloat* Damage = DamageTypes.Find(DamageTypeTag))
	{
		return Damage->GetValueAtLevel(Level);
	}
	
	return 0.f;
}

int32 UActiveDamageAbility::GetRoundedDamageAtLevel(int32 Level, FGameplayTag DamageTypeTag)
{
	return FMath::RoundToInt32(GetDamageAtLevel(Level, DamageTypeTag));
}
