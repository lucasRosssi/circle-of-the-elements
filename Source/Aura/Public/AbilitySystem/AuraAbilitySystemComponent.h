// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAbilityInfo;
class AAuraCharacterBase;
struct FAuraGameplayTags;
enum class EAbilityStatus : uint8;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_FourParams(
	FAbilityStateChanged,
	const FGameplayTag& /* AbilityTag */,
	const FGameplayTag& /* StatusTag */,
	const FGameplayTag& /* InputTag */,
	int32 /* AbilityLevel */
);
DECLARE_MULTICAST_DELEGATE_FourParams(
	FAbilityEquipped,
	const FGameplayTag& /* AbilityTag */,
	const FGameplayTag& /* StatusTag */,
	const FGameplayTag& /* InputTag */,
	const FGameplayTag& /* PreviousInputTag */
);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStateChanged AbilityStateChanged;
	FAbilityEquipped AbilityEquipped;

	void AddCharacterAbilities(
		const TArray<TSubclassOf<UGameplayAbility>>& InAbilities,
		const FGameplayTag& AbilityStatus
		);
	bool bStartupAbilitiesGiven = false;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ConfirmPressed();
	void CancelPressed();

	void ForEachAbility(const FForEachAbility& Delegate);

	UFUNCTION(BlueprintCallable)
	AAuraCharacterBase* GetAvatarCharacter();

	static void SetExclusiveGameplayTagFromSpec(
		FGameplayAbilitySpec& AbilitySpec,
		const FGameplayTag& GameplayTag,
		const FGameplayTag& ParentTag
		);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static void SetAbilityStatusFromSpec(
		FGameplayAbilitySpec& AbilitySpec,
		const FGameplayTag& StatusTag
		);
	static void SetInputTagFromSpec(
		FGameplayAbilitySpec& AbilitySpec,
		const FGameplayTag& InputTag
		);
	void RemoveInputTagFromAbilities(const FGameplayTag& InputTag);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	// void UpdateAbilityStatuses(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerSpendSkillPoint(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		const FGameplayTag& InputTag,
		const FGameplayTag& PreviousInputTag
		);

	FString GetDescriptionByAbilityTag(
		const UAbilityInfo* AbilityInfo,
		const FGameplayTag& AbilityTag
		);
	FString GetNextLevelDescriptionByAbilityTag(
		const UAbilityInfo* AbilityInfo,
		const FGameplayTag& AbilityTag
		);
	
protected:
	virtual void OnRep_ActivateAbilities() override;
	
	void EffectApplied(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayEffectSpec& EffectSpec,
		FActiveGameplayEffectHandle ActiveEffectHandle
	);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityState(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		const FGameplayTag& InputTag = FGameplayTag(),
		int32 AbilityLevel = 1
	);

private:
	UPROPERTY()
	AAuraCharacterBase* AvatarCharacter = nullptr;

};
