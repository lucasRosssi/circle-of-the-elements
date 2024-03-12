// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FTargetDataSignature,
	const FGameplayAbilityTargetDataHandle&,
	DataHandle
);

/**
 * 
 */
UCLASS()
class AURA_API UTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintCallable,
		Category = "Ability|Tasks",
		meta = (
			DisplayName = "Get Ability Target Data",
			HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"
		)
	)
	static UTargetData* CreateTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendMouseOrGamepadData();

	void OnTargetDataReplicatedCallback(
		const FGameplayAbilityTargetDataHandle& DataHandle,
		FGameplayTag ActivationTag
	);
};
