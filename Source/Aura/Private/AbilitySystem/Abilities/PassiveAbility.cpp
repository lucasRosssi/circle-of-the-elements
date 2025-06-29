// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/PassiveAbility.h"

UPassiveAbility::UPassiveAbility()
{
  NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
  AbilityTargetTeam = ETargetTeam::Self;
}

void UPassiveAbility::OnGiveAbility(
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilitySpec& Spec
)
{
	Super::OnGiveAbility(ActorInfo, Spec);
  
	if (bActivatesOnGranted && Spec.GetPrimaryInstance())
	{
	  CallActivateAbility(Spec.Handle, ActorInfo, Spec.GetPrimaryInstance()->GetCurrentActivationInfo());
	}
}
