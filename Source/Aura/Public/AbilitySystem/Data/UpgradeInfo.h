// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "Enums/CharacterName.h"
#include "UpgradeInfo.generated.h"

class UUpgradeAbility;
class UGameplayAbility;
class UUpgradeEffect;

USTRUCT(BlueprintType)
struct FAuraUpgradeInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UUpgradeEffect> GameplayEffect;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UUpgradeAbility> Ability;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MaxLevel = 1;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Resources", ForceInlineRow))
  TMap<FGameplayTag, FScalableFloat> Cost;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGameplayTagContainer Requirements;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<const UTexture2D> Icon = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FText Name = FText();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
  FText Description = FText();
	
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
  FText NextLevelDescription = FText();
  
};

USTRUCT(BlueprintType)
struct FUpgradeInfoParams
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FGameplayTag UpgradeTag = FGameplayTag();

  UPROPERTY(BlueprintReadWrite)
  FGameplayTag ElementTag = FGameplayTag();

  UPROPERTY(BlueprintReadWrite)
  ECharacterName CharacterName = ECharacterName::Undefined;
};

USTRUCT(BlueprintType)
struct FUpgradeListMapStruct
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
      Categories="Upgrades"
      ))
  TMap<FGameplayTag, FAuraUpgradeInfo> UpgradeList;
};

USTRUCT(BlueprintType)
struct FUpgradesMapStruct
{
  GENERATED_BODY()
	
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(
      NoResetToDefault,
      ForceInlineRow,
      Categories="Upgrades"
      ))
  TMap<FGameplayTag, FUpgradeListMapStruct> Elements;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UUpgradeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
  FAuraUpgradeInfo FindUpgradeInfoByTag(
    const FGameplayTag& UpgradeTag,
    bool bLogNotFound = false
  ) const;

  FUpgradesMapStruct FindCharacterUpgrades(
    ECharacterName CharacterName,
    bool bLogNotFound = false
  ) const;

  TMap<FGameplayTag, FAuraUpgradeInfo> FindCharacterUpgradesOfElement(
    ECharacterName CharacterName,
    const FGameplayTag& ElementTag,
    bool bLogNotFound = false
  ) const;

  FAuraUpgradeInfo FindUpgradeInfoWithParams(
    const FUpgradeInfoParams& Params,
    bool bLogNotFound = false
    ) const;
  
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="UpgradeInformation",
    meta=(NoResetToDefault)
    )
  TMap<ECharacterName, FUpgradesMapStruct> Upgrades;
};
