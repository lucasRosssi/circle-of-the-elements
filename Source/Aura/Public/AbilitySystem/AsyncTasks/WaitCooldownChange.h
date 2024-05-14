// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayTag& InCooldownTag
	);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveCooldownAdded(
		UAbilitySystemComponent* TargetASC,
		const FGameplayEffectSpec& SpecApplied,
		FActiveGameplayEffectHandle ActiveCooldownHandle
	);
};
