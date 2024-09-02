// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AActor* UActiveAbility::GetNextBounceTarget(AActor* HitTarget)
{
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
	return FMath::RoundToInt32(MaxHitCount.GetValueAtLevel(Level));
}

float UActiveAbility::GetEffectChangePerHitAtLevel_Implementation(int32 Level) const
{
	return EffectChangePerHit.GetValueAtLevel(Level);
}

bool UActiveAbility::IsActiveAbility_Implementation() const
{
	return true;
}

float UActiveAbility::GetMontagePlayRate() const
{
	return MontagePlayRate.GetValueAtLevel(GetAbilityLevel());
}

float UActiveAbility::GetAnimRootMotionTranslateScale() const
{
	return AnimRootMotionTranslateScale.GetValueAtLevel(GetAbilityLevel());
}

int32 UActiveAbility::GetMaxHitCount() const
{
	return FMath::RoundToInt32(MaxHitCount.GetValueAtLevel(GetAbilityLevel())); 
}

float UActiveAbility::GetEffectChangePerHit() const
{
	return EffectChangePerHit.GetValueAtLevel(GetAbilityLevel());
}

void UActiveAbility::ClearBounceHitTargets()
{
	BounceHitActors.Empty();
}
