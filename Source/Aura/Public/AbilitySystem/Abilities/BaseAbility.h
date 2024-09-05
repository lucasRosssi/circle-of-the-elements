// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/AbilityRechargeMode.h"
#include "Enums/TargetTeam.h"
#include "Interaction/AbilityInterface.h"
#include "BaseAbility.generated.h"

class AAuraCharacterBase;
struct FAuraAbilityInfo;

USTRUCT(BlueprintType)
struct FStatusEffectApplicationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="StatusEffects"))
	FGameplayTag StatusEffectTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Value = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Units="Seconds"))
	FScalableFloat Duration = 0.f;

	bool IsValid() const { return StatusEffectTag.IsValid(); }
};
/**
 * 
 */
UCLASS()
class AURA_API UBaseAbility : public UGameplayAbility, public IAbilityInterface
{
	GENERATED_BODY()

public:
	UBaseAbility();
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags) override;
	virtual bool CommitAbilityCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool ForceCooldown, FGameplayTagContainer* OptionalRelevantTags) override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void OnAbilityLevelUp(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec* Spec
		);

	// Ability Interface overrides
	virtual bool IsActiveAbility_Implementation() const override;
	virtual bool IsDamageAbility_Implementation() const override;
	virtual bool IsAreaEffectActorAbility_Implementation() const override;
	// END Ability Interface overrides
	
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetManaCostAtLevel(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedManaCostAtLevel(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetCooldownAtLevel(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedCooldownAtLevel(int32 Level) const;

	UFUNCTION(BlueprintPure, Category="Cooldowns", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetMaxChargesAtLevel(int32 Level) const;
	
	UGameplayEffect* GetChargesEffect();
	bool IsUsingCharges() const { return bUseCharges; }

	const FStatusEffectApplicationData& GetStatusEffectData() const { return StatusEffectData; }

	UFUNCTION(BlueprintPure, Category="Ability Effect", meta=(HidePin="Target", DefaultToSelf="Target"))
	virtual FAbilityParams MakeAbilityParamsFromDefaults(AActor* TargetActor = nullptr) const;

protected:
	UFUNCTION(BlueprintPure, Category="Ability")
	AAuraCharacterBase* GetAvatarCharacter();
	
	UFUNCTION(BlueprintPure, Category="Ability Info")
	FGameplayTag GetAbilityTag();

	UFUNCTION(BlueprintPure, Category="Ability Defaults|Targeting")
	float GetAreaInnerRadius() const;
	UFUNCTION(BlueprintPure, Category="Ability Defaults|Targeting")
	float GetAreaOuterRadius() const;
	UFUNCTION(BlueprintPure, Category="Cooldowns")
	int32 GetMaxCharges() const;

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

	UPROPERTY(
		EditDefaultsOnly, 
		BlueprintReadWrite,
		Category="Ability Defaults|Targeting",
		meta=(EditCondition="bIsAreaAbility", EditConditionHides)
		)
	FScalableFloat AreaInnerRadius = 200.f;
	
	UPROPERTY(
		EditDefaultsOnly, 
		BlueprintReadWrite,
		Category="Ability Defaults|Targeting",
		meta=(EditCondition="bIsAreaAbility", EditConditionHides)
		)
	FScalableFloat AreaOuterRadius = 200.f;
	
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
	bool bUseCharges = false;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bUseCharges", EditConditionHides)
		)
	TSubclassOf<UGameplayEffect> ChargesEffectClass;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bUseCharges", EditConditionHides)
		)
	FScalableFloat MaxCharges = 1;

	// How the ability charges will recover when the cooldown effect ends
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Cooldowns",
		meta=(EditCondition="bUseCharges", EditConditionHides)
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
	
	bool IsChargesModeActive() const;
	
	UPROPERTY()
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY()
	UGameplayEffect* ChargesEffect = nullptr;

	UPROPERTY()
	AAuraCharacterBase* AvatarCharacter = nullptr;
	
};
