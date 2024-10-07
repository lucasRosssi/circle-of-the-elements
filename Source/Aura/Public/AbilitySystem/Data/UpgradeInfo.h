// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "UpgradeInfo.generated.h"

class UGameplayAbility;
class UUpgradeEffect;
enum class ECharacterName : uint8;

USTRUCT(BlueprintType)
struct FAuraUpgradeInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UUpgradeEffect> UpgradeEffect;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UGameplayAbility> UpgradeAbility;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MaxLevel = 1;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Resources", ForceInlineRow))
  TMap<FGameplayTag, FScalableFloat> Cost;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGameplayTagContainer Requirements;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FText Name = FText();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
  FText Description = FText();
	
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
  FText NextLevelDescription = FText();
  
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
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="UpgradeInformation",
    meta=(NoResetToDefault)
    )
  TMap<ECharacterName, FUpgradesMapStruct> Upgrades;
};
