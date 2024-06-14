// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseAbility.generated.h"

struct FAuraAbilityInfo;
/**
 * 
 */
UCLASS()
class AURA_API UBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetManaCost(int32 Level);
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	float GetCooldown();
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedManaCost(int32 Level);
	UFUNCTION(BlueprintPure, Category="Resources", meta=(HidePin="Target", DefaultToSelf="Target"))
	int32 GetRoundedCooldown();
	
protected:
	UFUNCTION(BlueprintPure, Category="Ability Info", meta=(HidePin="Target", DefaultToSelf="Target"))
	FGameplayTag GetAbilityTag();

	virtual void FormatDescriptionAtLevel(FText& Description, int32 Level);

private:
	UPROPERTY(EditDefaultsOnly, Category="Cooldowns")
	float BaseCooldown = 0.f;

	UPROPERTY()
	FGameplayTag AbilityTag = FGameplayTag();
};
