// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UStatusEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UStatusEffect();

	FOnActiveGameplayEffectStackChange StackChange;
};
