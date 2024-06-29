// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
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
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility"))
	FGameplayTag StartupInputTag;
	
	// True if the ability should target accordingly to the player movement input direction 
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(EditCondition="bIsPlayerAbility"))
	bool bUsesMovementInputDirection = false;

	bool IsBounceModeActive() const { return bBounces; }
	int32 GetMaxBounceCountAtLevel(int32 Level) const;

protected:
	UFUNCTION(BlueprintCallable)
	void ClearBounceHitTargets();
	
	// True when a player uses the ability, false when its an AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	bool bIsPlayerAbility = false;

	// True when the ability is related to a weapon (uses a weapon socket)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults")
	bool bUsesWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults")
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	float MontagePlayRate = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	float AnimRootMotionTranslateScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	FName AbilitySocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults")
	TSubclassOf<ATargetingActor> TargetingActorClass;

	// True if the ability effect can bounce to a nearby target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode")
	bool bBounces = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode", meta=(EditCondition="bBounces"))
	int32 MaxBounceCount = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode", meta=(EditCondition="bBounces"))
	float BounceRadius = 500.f;
	// How much of the ability effect is changed per target hit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Mode", meta=(EditCondition="bBounces"))
	float BounceEffectChangePerHit = 0.f;

	UPROPERTY(BlueprintReadWrite, Category="Ability States")
	FName TargetingState = FName("TargetingState");
private:
	UPROPERTY()
	TArray<AActor*> BounceHitActors;

};
