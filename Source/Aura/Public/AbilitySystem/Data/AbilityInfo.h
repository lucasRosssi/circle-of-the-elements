// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "Enums/CharacterName.h"
#include "AbilityInfo.generated.h"

class ASpiritActor;
class UBaseAbility;

UENUM(BlueprintType)
enum class EDescriptionValueType : uint8
{
  Integer UMETA(DisplayName = "Integer"),
  Float UMETA(DisplayName = "Float"),
  Percent UMETA(DisplayName = "Percent"),

  MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FDescriptionData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  EDescriptionValueType ValueType = EDescriptionValueType::Integer;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FScalableFloat Value;
};

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<UBaseAbility> Ability;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  FGameplayTag AbilityTag = FGameplayTag();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  FGameplayTag ElementTag = FGameplayTag();

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

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<ASpiritActor> SpiritActor;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FText Name = FText();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MultiLine=true))
  FText Description = FText();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FDescriptionData> DescriptionData;

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
};

USTRUCT(BlueprintType)
struct FAbilityElementInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<ASpiritActor> ElementSpiritActorDefault;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(
      ShowOnlyInnerProperties,
      NoResetToDefault,
      TitleProperty="Name",
      ForceInlineRow,
      Categories="Abilities"
    )
  )
  TMap<FGameplayTag, FAuraAbilityInfo> AbilityList;
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
  GENERATED_BODY()

public:
#if WITH_EDITOR
  virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;;
#endif
  
  FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag) const;

  TMap<FGameplayTag, FAuraAbilityInfo> FindElementAbilities(const FGameplayTag& ElementTag) const;

  FAuraAbilityInfo FindAbilityInfoWithParams(const FAbilityInfoParams& Params) const;

  FAbilityElementInfo FindElementInfo(const FGameplayTag& ElementTag) const;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="AbilityInformation",
    meta=(
      NoResetToDefault,
      ForceInlineRow,
      Categories="Abilities.Element"
    )
  )
  TMap<FGameplayTag, FAbilityElementInfo> Abilities;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="AbilityInformation",
    meta=(NoResetToDefault, Categories="Abilities.Tier", ForceInlineRow)
  )
  TMap<FGameplayTag, int32> TierDropProbability;
};
