// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitChargesChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargesChangeSignature, int32, Charges);

/**
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitChargesChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FChargesChangeSignature ChargesChange;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"))
	static UWaitChargesChange* WaitForChargesChange(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayTag& InChargeTag
	);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	void OnChargeTagChanged(const FGameplayTag InChargeTag, int32 NewCount);
	void OnChargesAdded(
		UAbilitySystemComponent* TargetASC,
		const FGameplayEffectSpec& SpecApplied,
		FActiveGameplayEffectHandle ActiveChargeHandle
	);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag ChargeTag;
};
