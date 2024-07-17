// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/AttributeSetInterface.h"

FAbilityParams UActiveDamageAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
	
	FAbilityParams AbilityParams = Super::MakeAbilityParamsFromDefaults(TargetActor);
	
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return AbilityParams;
	
	float CurrentDamage = GetDamageAtLevel(GetAbilityLevel());
	AbilityParams.DamageParams.DamageEffectClass = DamageEffectClass;
	AbilityParams.DamageParams.DamageType = DamageType;
	AbilityParams.DamageParams.bApplyHitReact = bApplyHitReact;
	
	if (bIsComboSequence)
	{
		if (ComboIndex == ComboSequenceMontages.Num() - 1)
		{
			const float FinishEffectChange = ComboFinishMagnitude.GetValueAtLevel(GetAbilityLevel());
			CurrentDamage = CurrentDamage * (1.f + FinishEffectChange);
		}
		else
		{
			const float EffectChange = ComboMagnitude.GetValueAtLevel(GetAbilityLevel());
			CurrentDamage = CurrentDamage * (1.f + EffectChange * ComboIndex);
		}
	}

	const float DamageMultiplier = IAttributeSetInterface::Execute_GetDamageMultiplier(
		AbilityParams.SourceASC->GetOwnerActor()
	);
	
	AbilityParams.DamageParams.BaseDamage = CurrentDamage * DamageMultiplier;
	
	
	return AbilityParams;
}

FAbilityParams UActiveDamageAbility::ApplyEffectChangePerHitToAbilityParams(
	FAbilityParams& AbilityParams,
	int32 HitCount
	)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return AbilityParams;
	
	if (HitMode != EAbilityHitMode::Default && GetMaxHitCount() > 1)
	{
		AbilityParams.DamageParams.BaseDamage *= (1.f + GetEffectChangePerHit() * HitCount);
		AbilityParams.EffectParams.Value *= (1.f + GetEffectChangePerHit() * HitCount);
		AbilityParams.EffectParams.Duration *= (1.f + GetEffectChangePerHit() * HitCount);
	}

	return AbilityParams;
}

float UActiveDamageAbility::GetDamageAtLevel(int32 Level) const
{
	return Damage.GetValueAtLevel(Level);
}

int32 UActiveDamageAbility::GetRoundedDamageAtLevel(int32 Level) const
{
	return FMath::RoundToInt32(GetDamageAtLevel(Level));
}

float UActiveDamageAbility::GetComboMagnitude() const
{
	return ComboMagnitude.GetValueAtLevel(GetAbilityLevel());
}

float UActiveDamageAbility::GetComboFinishMagnitude() const
{
	return ComboFinishMagnitude.GetValueAtLevel(GetAbilityLevel());
}

UAnimMontage* UActiveDamageAbility::GetAnimMontage()
{
	return bIsComboSequence ? ComboSequenceMontages[ComboIndex] : MontageToPlay;
}
