// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/PassiveAbility.h"

UPassiveAbility::UPassiveAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
  AbilityTargetTeam = ETargetTeam::Self;
}

void UPassiveAbility::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (bActivatesOnGranted)	CallActivateAbility(Spec.Handle, ActorInfo,	Spec.ActivationInfo);
}
