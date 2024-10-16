// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/InteractionAbility.h"

#include "AIController.h"
#include "Interfaces/PlayerInterface.h"
#include "Player/AuraPlayerController.h"

UInteractionAbility::UInteractionAbility()
{
  InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
  bServerRespectsRemoteAbilityCancellation = false;
}

void UInteractionAbility::ActivateAbility(
  const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData
)
{
  Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

  const APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
  if (TriggerEventData)
  {
    if (AvatarPawn && AvatarPawn->Implements<UPlayerInterface>())
    {
      FPlayerInteractionData Data;
      Data.InteractionTag = TriggerEventData->EventTag;
      Data.AuraPlayerController = Cast<AAuraPlayerController>(AvatarPawn->GetController());
      PlayerActivateInteraction(Data);
    }
    else
    {
      FAIInteractionData Data;
      Data.InteractionTag = TriggerEventData->EventTag;
      Data.AIController = Cast<AAIController>(AvatarPawn->GetController());
      AIActivateInteraction(Data);
    }
  }
}
