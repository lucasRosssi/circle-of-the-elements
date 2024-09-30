// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "UpgradeInfo.generated.h"

class UUpgradeEffect;
enum class ECharacterName : uint8;

USTRUCT(BlueprintType)
struct FAuraUpgradeInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UUpgradeEffect> UpgradeClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  int32 MaxLevel = 1;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TMap<FGameplayTag, float> Cost;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGameplayTagContainer Requirements;
  
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
