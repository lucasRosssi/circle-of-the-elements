// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FAbilityParams UActiveDamageAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
	FAbilityParams AbilityParams = Super::MakeAbilityParamsFromDefaults(TargetActor);
	AbilityParams.DamageParams.DamageEffectClass = DamageEffectClass;
	AbilityParams.DamageParams.BaseDamage = GetDamageAtLevel(GetAbilityLevel());
	AbilityParams.DamageParams.DamageType = DamageType;
	AbilityParams.DamageParams.bApplyHitReact = bApplyHitReact;

	return AbilityParams;
}

void UActiveDamageAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle =  MakeOutgoingGameplayEffectSpec
	(DamageEffectClass, 1.f);
	
	const float ScaledDamage = GetDamageAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageSpecHandle,
		DamageType,
		ScaledDamage
	);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)
	);
}

float UActiveDamageAbility::GetDamageAtLevel(int32 Level) const
{
	return Damage.GetValueAtLevel(Level);
}

int32 UActiveDamageAbility::GetRoundedDamageAtLevel(int32 Level) const
{
	return FMath::RoundToInt32(GetDamageAtLevel(Level));
}
