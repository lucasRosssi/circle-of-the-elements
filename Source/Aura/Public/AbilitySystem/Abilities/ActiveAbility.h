// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Enums/AbilityActivationMode.h"
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
  virtual void ApplyUpgrade_Implementation(const FGameplayTag& UpgradeTag) override;
  virtual void RemoveUpgrade_Implementation(const FGameplayTag& UpgradeTag) override;
	// END Ability Interface overrides
  
	UFUNCTION(BlueprintCallable)
	AActor* GetNextRicochetTarget(AActor* HitTarget);

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
  UFUNCTION(BlueprintPure, Category="Ability Defaults|Mode|Actor Spawn")
  int32 GetActorCharges() const;
	
	UFUNCTION(BlueprintCallable)
	void ClearRicochetHitTargets();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Activation")
  EAbilityActivationMode ActivationMode = EAbilityActivationMode::Default;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Activation",
    meta=(
      EditCondition="ActivationMode == EAbilityActivationMode::Channeling",
      EditConditionHides,
      Units="s",
      ClampMin=0.f,
      UIMin=0.f,
      ClampMax=1.f,
      UIMax=1.f
      )
    )
  float ChannelingTick = 0.2f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Activation",
    meta=(
      EditCondition="ActivationMode == EAbilityActivationMode::Focus",
      EditConditionHides,
      Units="s",
      ClampMin=0.f,
      UIMin=0.f,
      ClampMax=5.f,
      UIMax=5.f
      )
    )
  float FocusTime = 1.f;
	
	// True when a player uses the ability, false when it is an AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(DisplayPriority=0))
	bool bIsPlayerAbility = false;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults")
	TObjectPtr<UAnimMontage> MontageToPlay;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FScalableFloat MontagePlayRate = 1.0f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalMontagePlayRate = 0.f;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FScalableFloat AnimRootMotionTranslateScale = 1.0f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalAnimRootMotionTranslateScale = 0.f;
  
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
  UPROPERTY(BlueprintReadWrite)
  int32 AdditionalMaxHitCount = 0;

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
  UPROPERTY(BlueprintReadWrite)
  float AdditionalEffectChangePerHit = 0.f;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="RangedHitMode == EAbilityHitMode::Ricochet", EditConditionHides)
		)
	FScalableFloat RicochetRadius = 500.f;
  UPROPERTY(BlueprintReadWrite)
  float AdditionalRicochetRadius = 0.f;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="RangedHitMode == EAbilityHitMode::Ricochet", EditConditionHides)
		)
	bool bCanRepeatTarget = false;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Mode|Actor Spawn"
    )
  bool bActorUsesCharges = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Mode|Actor Spawn",
    meta=(EditCondition="bActorUsesCharges", EditConditionHides)
    )
  FScalableFloat ActorCharges = 0.f;
  UPROPERTY(BlueprintReadWrite)
  int32 AdditionalActorCharges = 0;

private:
  bool CheckForClarityEffect(const FGameplayAbilityActorInfo* ActorInfo);
  
	UPROPERTY()
	TArray<AActor*> RicochetHitActors;

};
