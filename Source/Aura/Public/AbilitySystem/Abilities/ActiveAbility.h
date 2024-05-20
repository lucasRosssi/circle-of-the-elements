// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActiveAbility.generated.h"

class AMainPlayerController;
/**
 * 
 */
UCLASS()
class AURA_API UActiveAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;
	
	/*
	 * True if the ability should target accordingly to the player movement input direction 
	 */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	bool bUsesMovementInputDirection;
};
