// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ActiveAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

AActor* UActiveAbility::GetNextBounceTarget(AActor* HitTarget)
{
	TArray ActorsToIgnore({GetAvatarActorFromActorInfo()});
	ActorsToIgnore.Append(BounceHitActors);
	AActor* NextTarget = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		HitTarget,
		BounceRadius,
		AbilityTargetTeam == ETargetTeam::ETT_Both ? AbilityTargetTeam : ETargetTeam::ETT_Friends,
		ActorsToIgnore
		);

	BounceHitActors.Add(HitTarget);
	return NextTarget;
}

int32 UActiveAbility::GetMaxBounceCountAtLevel(int32 Level) const
{
	return MaxBounceCount + Level - 1;
}

void UActiveAbility::ClearBounceHitTargets()
{
	BounceHitActors.Empty();
}
