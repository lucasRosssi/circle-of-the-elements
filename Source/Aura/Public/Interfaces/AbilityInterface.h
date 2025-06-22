// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/AbilityHitMode.h"
#include "UObject/Interface.h"
#include "AbilityInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAbilityInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EAbilityHitMode GetHitMode() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMaxHitCountAtLevel(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetEffectChangePerHitAtLevel(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetRoundedDamageAtLevel(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetBeamTickPeriod() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAreaEffectDurationAtLevel(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetPeriodAtLevel(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetDamageTypeTag() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsActiveAbility() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDamageAbility() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsAreaEffectActorAbility() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  int32 GetRoundedHealAtLevel(int32 Level) const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  FName GetActiveAbilitySocketName() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  bool IsAbilityUsingWeapon() const;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void ApplyUpgrade(const FGameplayTag& UpgradeTag);
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void RemoveUpgrade(const FGameplayTag& UpgradeTag);
};
