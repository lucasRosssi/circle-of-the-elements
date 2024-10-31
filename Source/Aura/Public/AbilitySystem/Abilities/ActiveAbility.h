// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Enums/AbilityHitMode.h"
#include "ActiveAbility.generated.h"

class ATargetingActor;
class AAuraPlayerController;

/**
 * 
 */
UCLASS()
class AURA_API UActiveAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
  virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;
  virtual bool CommitAbilityCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;
  
	// Ability Interface overrides
	virtual EAbilityHitMode GetHitMode_Implementation() const override { return RangedHitMode; }
	virtual int32 GetMaxHitCountAtLevel_Implementation (int32 Level) const override;
	virtual float GetEffectChangePerHitAtLevel_Implementation(int32 Level) const override;
	virtual bool IsActiveAbility_Implementation() const override;
	// END Ability Interface overrides
  
	UFUNCTION(BlueprintCallable)
	AActor* GetNextBounceTarget(AActor* HitTarget);

  // Starting input for the player, when the ability is granted
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=1))
	FGameplayTag StartupInputTag;
	// True if the ability should target accordingly to the player movement input direction 
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=2))
	bool bCanHoldInput = false;
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=3))
	bool bUsesMovementInputDirection = false;


protected:
	UFUNCTION(BlueprintPure, Category="Ability Defaults")
	void GetMontageParams(UAnimMontage*& Montage, float& PlayRate, float& RootMotionScale) const;
  virtual UAnimMontage* GetAnimMontage() const;
	UFUNCTION(BlueprintPure, Category="Ability Defaults")
	float GetMontagePlayRate() const;
	UFUNCTION(BlueprintPure, Category="Ability Defaults")
	float GetAnimRootMotionTranslateScale() const;
	
	UFUNCTION(BlueprintPure, Category="Ability Defaults|Mode")
	int32 GetMaxHitCount() const;
	UFUNCTION(BlueprintPure, Category="Ability Defaults|Mode")
	float GetEffectChangePerHit() const;
	
	UFUNCTION(BlueprintCallable)
	void ClearBounceHitTargets();
	
	// True when a player uses the ability, false when it is an AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(DisplayPriority=0))
	bool bIsPlayerAbility = false;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults")
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FScalableFloat MontagePlayRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FScalableFloat AnimRootMotionTranslateScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FName AbilitySocketName;
  // True when the ability is related to a weapon (uses a weapon socket)
  UPROPERTY(EditDefaultsOnly,	BlueprintReadOnly, Category="Ability Defaults")
  bool bUseWeaponSocket;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Targeting")
	TSubclassOf<ATargetingActor> TargetingActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode")
	EAbilityHitMode RangedHitMode = EAbilityHitMode::Default;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="RangedHitMode != EAbilityHitMode::Default",
			EditConditionHides
			)
		)
	FScalableFloat MaxHitCount = 1.f;

	// How much of the ability effect is changed per target hit
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="RangedHitMode != EAbilityHitMode::Default",
			EditConditionHides
			)
		)
	FScalableFloat EffectChangePerHit = 0.f;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="RangedHitMode == EAbilityHitMode::Bounce", EditConditionHides)
		)
	FScalableFloat BounceRadius = 500.f;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="RangedHitMode == EAbilityHitMode::Bounce", EditConditionHides)
		)
	bool bCanRepeatTarget = false;

private:
  bool CheckForClarityEffect(const FGameplayAbilityActorInfo* ActorInfo);
  
	UPROPERTY()
	TArray<AActor*> BounceHitActors;

};
