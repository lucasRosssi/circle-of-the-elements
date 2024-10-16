// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InteractionAbility.generated.h"

class AAuraPlayerState;
class AAIController;
class AAuraPlayerController;

USTRUCT(BlueprintType)
struct FPlayerInteractionData
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly)
  FGameplayTag InteractionTag;
  
  UPROPERTY(BlueprintReadOnly)
  AAuraPlayerController* AuraPlayerController = nullptr;
};

USTRUCT(BlueprintType)
struct FAIInteractionData
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly)
  FGameplayTag InteractionTag;
  
  UPROPERTY(BlueprintReadOnly)
  AAIController* AIController = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UInteractionAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
  UInteractionAbility();

  virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
  UFUNCTION(BlueprintImplementableEvent)
  void PlayerActivateInteraction(const FPlayerInteractionData& Data);
  UFUNCTION(BlueprintImplementableEvent)
  void AIActivateInteraction(const FAIInteractionData& Data);
};
