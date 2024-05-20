// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveAbility.h"
#include "ActiveDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UActiveDamageAbility : public UActiveAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
