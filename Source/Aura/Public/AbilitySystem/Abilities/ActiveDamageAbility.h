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

	UFUNCTION(BlueprintPure, meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetDamageAtLevel(int32 Level, FGameplayTag DamageTypeTag);

	UFUNCTION(BlueprintPure, meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedDamageAtLevel(int32 Level, FGameplayTag DamageTypeTag);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
