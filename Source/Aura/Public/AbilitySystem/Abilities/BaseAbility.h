// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/AbilityRechargeMode.h"
#include "Enums/TargetTeam.h"
#include "BaseAbility.generated.h"

struct FAuraAbilityInfo;

USTRUCT(BlueprintType)
struct FStatusEffectApplicationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="StatusEffects"))
	FGameplayTag StatusEffectTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Seconds"))
	float Duration = 0.f;

	bool IsValid() const { return StatusEffectTag.IsValid(); }
};
/**
 * 
 */
UCLASS()
class AURA_API UBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;
	virtual bool CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags) override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetManaCost(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetCooldown(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedManaCost(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedCooldown(int32 Level) const;

	UGameplayEffect* GetChargesEffect(); 

	UFUNCTION(BlueprintPure, Category="Ability Effect", meta=(HidePin="Target", DefaultToSelf="Target"))
	virtual FAbilityParams MakeAbilityParamsFromDefaults(AActor* TargetActor = nullptr) const;

protected:
	UFUNCTION(BlueprintPure, Category="Ability Info", meta=(HidePin="Target", DefaultToSelf="Target"))
	FGameplayTag GetAbilityTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Status Effects")
	FStatusEffectApplicationData StatusEffectData;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Targeting",
		meta=(DisplayPriority=0)
		)
	ETargetTeam AbilityTargetTeam = ETargetTeam::Enemies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Targeting")
	bool bIsAreaAbility = false;

	UPROPERTY(EditDefaultsOnly, 
	BlueprintReadWrite,
	Category="Ability Defaults|Targeting",
	meta=(EditCondition="bIsAreaAbility", EditConditionHides)
	)
	float AreaInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, 
	BlueprintReadWrite,
	Category="Ability Defaults|Targeting",
	meta=(EditCondition="bIsAreaAbility", EditConditionHides)
	)
	float AreaOuterRadius = 300.f;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Targeting")
	FVector AreaOrigin = FVector::ZeroVector;
	
/*
True if the ability will be limited by Charges economy instead of cooldown effect directly.
The instancing policy MUST be set to "Instanced Per Actor", so if the ability needs a clean
state when activating, we have to clean it first thing when activating or clean it in the
OnEndAbility event. Or make the ability not constantly dependent on variables.
*/
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="InstancingPolicy == EGameplayAbilityInstancingPolicy::InstancedPerActor")
		)
	bool bHasCharges = false;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bHasCharges", EditConditionHides)
		)
	TSubclassOf<UGameplayEffect> ChargesEffectClass;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bHasCharges", EditConditionHides, ClampMin=1, ClampMax=5, UIMin=1, UIMax=5)
		)
	int32 Charges = 1;

	// How the ability charges will recover when the cooldown effect ends
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bHasCharges", EditConditionHides)
		)
	EAbilityRechargeMode RechargeMode = EAbilityRechargeMode::OneCharge;

	UPROPERTY(EditDefaultsOnly, Category="Cooldowns")
	FScalableFloat Cooldown;

private:
	void HandleCooldownRecharge(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo& ActivationInfo
		);
	
	bool IsChargesModeActive() const { return bHasCharges && Charges > 1 && IsValid(ChargesEffectClass); }
	
	UPROPERTY()
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY()
	UGameplayEffect* ChargesEffect;
	
};
