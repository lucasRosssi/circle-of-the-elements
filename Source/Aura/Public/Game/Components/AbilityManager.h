// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/Components/AuraSystemComponent.h"
#include "AbilityManager.generated.h"

struct FAbilityInfoParams;
enum class ECharacterName : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySelected, FAuraAbilityInfo, AbilityInfo);

/**
 * 
 */
UCLASS()
class AURA_API UAbilityManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
	UAbilityManager();
	
	UFUNCTION(BlueprintPure, Category="Manager|Ability")
	TMap<FGameplayTag, FAuraAbilityInfo> GetElementAbilities(
		ECharacterName CharacterName,
		const FGameplayTag ElementTag
		);
	UFUNCTION(BlueprintPure, Category="Manager|Ability")
	FAuraAbilityInfo GetAbilityInfo(const FAbilityInfoParams& Params);
	UFUNCTION(BlueprintCallable, Category="Manager|Ability")
	TArray<FAuraAbilityInfo> RandomizeElementAbilities(
		ECharacterName CharacterName,
		const FGameplayTag& ElementTag,
		int32 Amount = 3
		);
	UFUNCTION(BlueprintCallable, Category="Manager|Ability")
	TArray<FAuraAbilityInfo> GetAbilitiesFromBag(const FGameplayTag& ElementTag);

	UFUNCTION(BlueprintCallable, Category="Manager|Ability|Description")
	void GetAbilityFormattedTexts(
		const FAuraAbilityInfo& AbilityInfo,
		FText& AbilityName,
		FText& AbilityDescription,
		FText& AbilityDetails
		);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Manager|Ability")
	FOnAbilitySelected OnAbilitySelectedDelegate;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Manager|Ability")
	bool bOverrideAbilities = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	FGameplayTagContainer AcquiredAbilities;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	FGameplayTagContainer BlockedAbilities;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Ability",
		meta=(EditCondition="bOverrideAbilities", ForceInlineRow)
		)
	TMap<FGameplayTag, FGameplayTagContainer> AbilitiesBag;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Manager|Ability",
		meta=(EditCondition="bOverrideAbilities", ForceInlineRow)
		)
	TMap<FGameplayTag, FGameplayTagContainer> ElementalTierPool;

private:
	void AssignNextAbilities();
	FGameplayTag RandomizeTier(const TMap<FGameplayTag, int32>& TierMap);
	TMap<FGameplayTag, FAuraAbilityInfo> GetRemainingElementAbilities(
		ECharacterName CharacterName,
		const FGameplayTag ElementTag
		);
	TArray<FAuraAbilityInfo> GetRemainingTierAbilities(
		const FGameplayTag& TierTag,
		const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities 
		);
	TMap<FGameplayTag, int32> GetAvailableTiers(const FGameplayTag& ElementTag);
};
