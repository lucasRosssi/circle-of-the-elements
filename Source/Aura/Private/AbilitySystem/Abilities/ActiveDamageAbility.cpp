// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FAbilityParams UActiveDamageAbility::MakeAbilityParamsFromDefaults(AActor* TargetActor) const
{
	float CurrentDamage = GetDamageAtLevel(GetAbilityLevel());
	FAbilityParams AbilityParams = Super::MakeAbilityParamsFromDefaults(TargetActor);
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
	
	AbilityParams.DamageParams.BaseDamage = CurrentDamage;
	
	
	return AbilityParams;
}

void UActiveDamageAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ComboIndex = 0;
}

void UActiveDamageAbility::GetMontageParams(
	UAnimMontage*& Montage,
	float& PlayRate,
	float& RootMotionScale
	) const
{
	Montage = bIsComboSequence ? ComboSequenceMontages[ComboIndex] : MontageToPlay;
	PlayRate = MontagePlayRate.GetValueAtLevel(GetAbilityLevel());
	RootMotionScale = AnimRootMotionTranslateScale.GetValueAtLevel(GetAbilityLevel());
}

bool UActiveDamageAbility::IsDamageAbility_Implementation() const
{
	return true;
}

FGameplayTag UActiveDamageAbility::GetDamageTypeTag_Implementation() const
{
	return DamageType;
}

FAbilityParams UActiveDamageAbility::ApplyEffectChangePerHitToAbilityParams(
	FAbilityParams& AbilityParams,
	int32 HitCount
	)
{
	if (RangedHitMode != EAbilityHitMode::Default && GetMaxHitCount() > 1)
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

int32 UActiveDamageAbility::GetRoundedDamageAtLevel_Implementation(int32 Level) const
{
	return FMath::RoundToInt32(GetDamageAtLevel(Level));
}

void UActiveDamageAbility::HandleComboSequence()
{
	if (!bComboInputPressed) return;

	ComboIndex += 1;
	bComboInputPressed = false;

	if (!ComboSequenceMontages.IsValidIndex(ComboIndex)) return;

	OnNextComboSequence();
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
