// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "Enums/CharacterName.h"
#include "AbilityInfo.generated.h"

class UBaseAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBaseAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Abilities"))
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Abilities.Tier"))
	FGameplayTag TierTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Charges"))
	FGameplayTag ChargesTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Abilities"))
	FGameplayTagContainer AbilitiesRequirement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
	FText Description = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
	FText NextLevelDescription = FText();
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FScalableFloat> DescriptionValues;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FScalableFloat> DescriptionPercents;

  bool IsValid() const { return AbilityTag.IsValid(); }
};

USTRUCT(BlueprintType)
struct FAbilityInfoParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ElementTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	ECharacterName HeroName = ECharacterName::Undefined;
};

USTRUCT(BlueprintType)
struct FAbilityListMapStruct
{
	GENERATED_BODY()

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(
			ShowOnlyInnerProperties,
			NoResetToDefault,
			TitleProperty="Name",
			ForceInlineRow,
			Categories="Abilities"
			))
	TMap<FGameplayTag, FAuraAbilityInfo> AbilityList;
};

USTRUCT(BlueprintType)
struct FElementsMapStruct
{
	GENERATED_BODY()
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		meta=(
			NoResetToDefault,
			ForceInlineRow,
			Categories="Abilities.Element"
			))
	TMap<FGameplayTag, FAbilityListMapStruct> Elements;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAbilityInfo FindAbilityInfoByTag(
		const FGameplayTag& AbilityTag,
		bool bLogNotFound = false
	) const;

	FElementsMapStruct FindCharacterAbilities(
		ECharacterName CharacterName,
		bool bLogNotFound = false
	) const;

	TMap<FGameplayTag, FAuraAbilityInfo> FindCharacterAbilitiesOfElement(
		ECharacterName CharacterName,
		const FGameplayTag& ElementTag,
		bool bLogNotFound = false
	) const;

	FAuraAbilityInfo FindAbilityInfoWithParams(
		const FAbilityInfoParams& Params,
		bool bLogNotFound = false
		) const;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="AbilityInformation",
		meta=(NoResetToDefault)
		)
	TMap<ECharacterName, FElementsMapStruct> Abilities;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="AbilityInformation",
		meta=(NoResetToDefault, Categories="Abilities.Tier", ForceInlineRow)
		)
	TMap<FGameplayTag, int32> TierDropProbability;
};
