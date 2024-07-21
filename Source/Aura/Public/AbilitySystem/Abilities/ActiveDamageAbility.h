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
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	FAbilityParams ApplyEffectChangePerHitToAbilityParams(
		UPARAM(ref) FAbilityParams& AbilityParams,
		int32 HitCount
		);
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Ability Defaults|Damage")
	float GetDamageAtLevel(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Ability Defaults|Damage")
	int32 GetRoundedDamageAtLevel(int32 Level) const;
	
protected:
	UFUNCTION(BlueprintPure, Category="AbilityDefaults")
	UAnimMontage* GetAnimMontage();

	UFUNCTION(BlueprintPure, Category="Ability Defaults|Combo")
	float GetComboMagnitude() const;

	UFUNCTION(BlueprintPure, Category="Ability Defaults|Combo")
	float GetComboFinishMagnitude() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=( Categories="Damage" ))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Status Effects")
	bool bApplyHitReact = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Combo")
	bool bIsComboSequence = false;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Combo",
		meta=(EditCondition="bIsComboSequence")
		)
	TArray<TObjectPtr<UAnimMontage>> ComboSequenceMontages;

	// How much each combo step increases/decrease the ability effect
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Combo",
		meta=(EditCondition="bIsComboSequence")
		)
	FScalableFloat ComboMagnitude = 0.f;

	// How much the last combo step increases/decrease the ability effect
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Combo",
		meta=(EditCondition="bIsComboSequence")
		)
	FScalableFloat ComboFinishMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Combo")
	bool bComboInputPressed = false;

	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Combo")
	int32 ComboIndex = 0;
};
