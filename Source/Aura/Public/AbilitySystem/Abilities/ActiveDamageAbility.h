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
	virtual FAbilityParams MakeAbilityParamsFromDefaults(AActor* TargetActor = nullptr) const 
	override;
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetDamageAtLevel(int32 Level) const;

	UFUNCTION(BlueprintPure, meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedDamageAtLevel(int32 Level) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=( Categories="Damage" ))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Status Effects")
	bool bApplyHitReact = true;
};
