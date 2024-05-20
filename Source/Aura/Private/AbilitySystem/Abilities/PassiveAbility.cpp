// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/PassiveAbility.h"

void UPassiveAbility::OnGiveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec
)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	CallActivateAbility(
		Spec.Handle,
		ActorInfo,
		Spec.ActivationInfo
	);
}
