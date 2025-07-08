// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilityManager.generated.h"

class AAuraPlayerState;
class UAuraAbilitySystemComponent;
struct FAbilityInfoParams;
enum class ECharacterName : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySelected, const FAuraAbilityInfo&, AbilityInfo);

/**
 * 
 */
UCLASS()
class AURA_API UAbilityManager : public UAuraSystemComponent
{
  GENERATED_BODY()

public:
  UAbilityManager();

  FGameplayAbilitySpec GiveAbility(
    UAuraAbilitySystemComponent* AuraASC,
    const FAuraAbilityInfo& AbilityInfo,
    int32 Level = 1,
    const FGameplayTag& InputTag = FGameplayTag()
  );
  void RemoveAbility(
    UAuraAbilitySystemComponent* AuraASC,
    const FGameplayTag& AbilityTag
  );
  void GiveAcquiredAbilities(AActor* Actor);

  UFUNCTION(BlueprintPure, Category="Manager|Ability")
  TMap<FGameplayTag, FAuraAbilityInfo> GetElementAbilities(
    const FGameplayTag ElementTag
  );
  UFUNCTION(BlueprintPure, Category="Manager|Ability")
  FAuraAbilityInfo GetAbilityInfo(const FAbilityInfoParams& Params);
  UFUNCTION(BlueprintPure, Category="Manager|Ability")
  FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag);
  UFUNCTION(BlueprintCallable, Category="Manager|Ability")
  TArray<FAuraAbilityInfo> RandomizeElementAbilities(
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

  UFUNCTION(BlueprintCallable, Category="Manager|Ability")
  void SelectAbilityReward(
    const FGameplayTag& ElementTag,
    const FAuraAbilityInfo& AbilityInfo
  );

  UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Manager|Ability")
  FOnAbilitySelected OnAbilitySelectedDelegate;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Manager|Ability")
  bool bInitializeOnBeginPlay = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Manager|Ability",
    meta=(
      Categories="Abilities.Active.Aura,Abilities.Active.Vilkar,Abilities.Passive.Aura,Abilities.Passive.Vilkar",
      ForceInlineRow
    )
  )
  TMap<FGameplayTag, int32> AcquiredAbilities;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Manager|Ability",
    meta=(Categories="Abilities.Active.Aura,Abilities.Active.Vilkar,Abilities.Passive.Aura,Abilities.Passive.Vilkar")
  )
  FGameplayTagContainer BlockedAbilities;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Manager|Ability")
  bool bOverrideAbilitiesContainer = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Manager|Ability",
    meta=(EditCondition="bOverrideAbilitiesContainer", ForceInlineRow, Categories=
      "Abilities.Active,Abilities.Passive,Abilities.Element")
  )
  TMap<FGameplayTag, FGameplayTagContainer> AbilitiesContainer;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Manager|Ability",
    meta=(EditCondition="bOverrideAbilitiesContainer", ForceInlineRow, Categories="Abilities.Element,Abilities.Tier")
  )
  TMap<FGameplayTag, FGameplayTagContainer> ElementalTierPool;

private:
  void AssignInitialAbilities();
  FGameplayTag RandomizeTier(const TMap<FGameplayTag, int32>& TierMap);
  TMap<FGameplayTag, FAuraAbilityInfo> GetRemainingElementAbilities(
    const FGameplayTag ElementTag
  );
  TArray<FAuraAbilityInfo> GetRemainingTierAbilities(
    const FGameplayTag& TierTag,
    const TMap<FGameplayTag, FAuraAbilityInfo>& ElementAbilities
  );
  TMap<FGameplayTag, int32> GetAvailableTiers(const FGameplayTag& ElementTag);
  void OnNoAbilitiesLeft(const FGameplayTag& ElementTag);

  FGameplayTag GetAvailableInputTag(UAuraAbilitySystemComponent* AuraASC);

  TMap<FGameplayTag, FGameplayTagContainer> OverridenAbilitiesContainer;
};
