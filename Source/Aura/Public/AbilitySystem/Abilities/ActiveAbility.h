// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Enums/AbilityHitMode.h"
#include "ActiveAbility.generated.h"

class ATargetingActor;
class AMainPlayerController;
/**
 * 
 */
UCLASS()
class AURA_API UActiveAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(DefaultToSelf="Target", HidePin="Target"))
	AActor* GetNextBounceTarget(AActor* HitTarget);
	
	// Starting input for the player, when the ability is granted
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=1))
	FGameplayTag StartupInputTag;
	// True if the ability should target accordingly to the player movement input direction 
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=2))
	bool bCanHoldInput = false;
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility", DisplayPriority=3))
	bool bUsesMovementInputDirection = false;

	bool IsBounceModeActive() const { return HitMode == EAbilityHitMode::Bounce; }
	int32 GetMaxBounceCountAtLevel(int32 Level) const;

protected:
	UFUNCTION(BlueprintCallable)
	void ClearBounceHitTargets();
	
	// True when a player uses the ability, false when its an AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(DisplayPriority=0))
	bool bIsPlayerAbility = false;

	// True when the ability is related to a weapon (uses a weapon socket)
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Weapon",
		meta=(DisplayPriority=0)
		)
	bool bUsesWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults")
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	float MontagePlayRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	float AnimRootMotionTranslateScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FName AbilitySocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Targeting")
	TSubclassOf<ATargetingActor> TargetingActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode")
	EAbilityHitMode HitMode = EAbilityHitMode::Default;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="HitMode == EAbilityHitMode::Bounce",
			EditConditionHides,
			ClampMin=0,
			UIMin=0
			)
		)
	int32 MaxBounceCount = 0;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="HitMode == EAbilityHitMode::Bounce", EditConditionHides)
		)
	float BounceRadius = 500.f;
	// How much of the ability effect is changed per target hit
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="HitMode == EAbilityHitMode::Bounce",
			EditConditionHides,
			Units="Percent"
			)
		)
	float BounceEffectChangePerHit = 0.f;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="HitMode == EAbilityHitMode::Penetration",
			EditConditionHides,
			ClampMin=0,
			UIMin=0
			)
		)
	int32 MaxPenetrationCount = 0;
	// How much of the ability effect is changed per target hit
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(
			EditCondition="HitMode == EAbilityHitMode::Penetration",
			EditConditionHides,
			Units="Percent"
			)
		)
	float PenetrationEffectChangePerHit = 0.f;

private:
	UPROPERTY()
	TArray<AActor*> BounceHitActors;

	UPROPERTY()
	int32 PenetrationCount = 0;

};
