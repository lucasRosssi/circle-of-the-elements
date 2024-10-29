// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interfaces/AttributeSetInterface.h"

AActor* UActiveAbility::GetNextBounceTarget(AActor* HitTarget)
{
  if (!IsValid(HitTarget)) return nullptr;
  
	TArray ActorsToIgnore({GetAvatarActorFromActorInfo()});
	ActorsToIgnore.Append(BounceHitActors);
	AActor* NextTarget = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		HitTarget,
		BounceRadius.GetValueAtLevel(GetAbilityLevel()),
		AbilityTargetTeam == ETargetTeam::Both ? AbilityTargetTeam : ETargetTeam::Friends,
		ActorsToIgnore
		);

	BounceHitActors.Add(HitTarget);
	return NextTarget;
}

int32 UActiveAbility::GetMaxHitCountAtLevel_Implementation(int32 Level) const
{
	return MaxHitCount.AsInteger(Level);
}

float UActiveAbility::GetEffectChangePerHitAtLevel_Implementation(int32 Level) const
{
	return EffectChangePerHit.GetValueAtLevel(Level);
}

bool UActiveAbility::IsActiveAbility_Implementation() const
{
	return true;
}

void UActiveAbility::GetMontageParams(
	UAnimMontage*& Montage,
	float& PlayRate,
	float& RootMotionScale
	) const
{
	Montage = GetAnimMontage();
	PlayRate = GetMontagePlayRate();
	RootMotionScale = GetAnimRootMotionTranslateScale();
}

UAnimMontage* UActiveAbility::GetAnimMontage() const
{
  return MontageToPlay;
}

float UActiveAbility::GetMontagePlayRate() const
{
  const float ActionSpeed = IAttributeSetInterface::Execute_GetActionSpeed(GetOwningActorFromActorInfo());
	return MontagePlayRate.GetValueAtLevel(GetAbilityLevel()) * ActionSpeed;
}

float UActiveAbility::GetAnimRootMotionTranslateScale() const
{
	return AnimRootMotionTranslateScale.GetValueAtLevel(GetAbilityLevel());
}

int32 UActiveAbility::GetMaxHitCount() const
{
	return MaxHitCount.AsInteger(GetAbilityLevel()); 
}

float UActiveAbility::GetEffectChangePerHit() const
{
	return EffectChangePerHit.GetValueAtLevel(GetAbilityLevel());
}

void UActiveAbility::ClearBounceHitTargets()
{
	BounceHitActors.Empty();
}
