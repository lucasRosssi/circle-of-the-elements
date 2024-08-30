// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/Components/AuraSystemComponent.h"
#include "AbilityManager.generated.h"

struct FAbilityInfoParams;
enum class ECharacterName : uint8;
/**
 * 
 */
UCLASS()
class AURA_API UAbilityManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
	UAbilityManager();
	
	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, FAuraAbilityInfo> GetElementAbilities(
		ECharacterName CharacterName,
		const FGameplayTag ElementTag
		);
	UFUNCTION(BlueprintPure)
	FAuraAbilityInfo GetAbilityInfo(const FAbilityInfoParams& Params);
	UFUNCTION(BlueprintPure)
	TArray<FAuraAbilityInfo> RandomizeElementAbilities(
		ECharacterName CharacterName,
		const FGameplayTag& ElementTag,
		int32 Amount = 3
		);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	bool bOverrideAbilities = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	FGameplayTagContainer AcquiredAbilities;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	FGameplayTagContainer BlockedAbilities;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	TMap<FGameplayTag, FGameplayTagContainer> AbilitiesBag;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Ability",
		meta=(EditCondition="bOverrideAbilities")
		)
	TMap<FGameplayTag, FGameplayTagContainer> ElementalTierPool;

private:
	FGameplayTag RandomizeTier(const TMap<FGameplayTag, int32>& TierMap);
	TArray<FAuraAbilityInfo> GetRemainingTierAbilities(
		const FGameplayTag& TierTag,
		const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities 
		);
	TMap<FGameplayTag, int32> GetAvailableTiers(const FGameplayTagContainer* TierPool);
};
