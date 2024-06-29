// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "PassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPassiveAbility : public UBaseAbility
{
	GENERATED_BODY()
public:
	UPassiveAbility();
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

private:
	UPROPERTY(EditDefaultsOnly)
	bool bActivatesOnGranted = false;
};
