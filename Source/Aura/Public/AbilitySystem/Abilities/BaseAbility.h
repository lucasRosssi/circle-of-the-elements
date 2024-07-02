// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
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
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetManaCost(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetCooldown(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedManaCost(int32 Level) const;
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedCooldown(int32 Level) const;

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
	ETargetTeam AbilityTargetTeam = ETargetTeam::ETT_Enemies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Targeting")
	bool bIsAreaAbility = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Targeting")
	float AreaInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability Defaults|Targeting")
	float AreaOuterRadius = 300.f;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Targeting")
	FVector AreaOrigin = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category="Cooldowns")
	FScalableFloat Cooldown;

private:
	UPROPERTY()
	FGameplayTag AbilityTag = FGameplayTag();
};
