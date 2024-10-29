// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "Enums/CharacterName.h"
#include "Enums/Region.h"
#include "AuraSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveInfo
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FString SlotName = FString();
  UPROPERTY(BlueprintReadWrite)
  int32 SlotIndex = -1;
  UPROPERTY(BlueprintReadWrite)
  FString PlayerName = FString("Player");
  UPROPERTY(BlueprintReadWrite)
  ECharacterName HeroName = ECharacterName::Undefined;
  UPROPERTY(BlueprintReadWrite)
  ERegion RegionName = ERegion::Undefined;

  bool IsValid() const
  {
    return !SlotName.IsEmpty() && SlotIndex >= 0 && HeroName != ECharacterName::Undefined;
  }
};

USTRUCT(BlueprintType)
struct FPlayerStateSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  int32 AttributePoints = 0;
  UPROPERTY(BlueprintReadWrite)
  int32 SkillPoints = 0;
  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, int32> Resources = TMap<FGameplayTag, int32>();

  void Reset()
  {
    AttributePoints = 0;
    SkillPoints = 0;
  }
};

USTRUCT(BlueprintType)
struct FAttributeSetSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, float> Attributes = TMap<FGameplayTag, float>();

  void Reset()
  {
    for (auto& [Attribute, Value] : Attributes)
    {
      Value = 10;
    }
  }
};

USTRUCT(BlueprintType)
struct FAbilityManagerSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, int32> AcquiredAbilities = TMap<FGameplayTag, int32>();
  UPROPERTY(BlueprintReadWrite)
  FGameplayTagContainer BlockedAbilities = FGameplayTagContainer();
  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, FGameplayTagContainer> AbilitiesContainer = TMap<FGameplayTag, FGameplayTagContainer>();
  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, FGameplayTagContainer> ElementalTierPool = TMap<FGameplayTag, FGameplayTagContainer>();

  UPROPERTY(BlueprintReadWrite)
  bool bIsStarting = true;

  void Reset()
  {
    AcquiredAbilities.Empty();
    BlockedAbilities = FGameplayTagContainer();
    AbilitiesContainer.Empty();
    ElementalTierPool.Empty();
    
    bIsStarting = true;
  }
};

USTRUCT(BlueprintType)
struct FAbilityInputSave
{
  GENERATED_BODY()

  // Keys: Input tags; Values: Ability tags
  UPROPERTY(BlueprintReadWrite, meta=(Categories="Abilities.Active,InputTag.1,InputTag.2,InputTag.3,InputTag.4"))
  TMap<FGameplayTag, FGameplayTag> Inputs = TMap<FGameplayTag, FGameplayTag>();

  FGameplayTag FindAbilityInput(const FGameplayTag& AbilityTag)
  {
    for (auto& [Input, Ability] : Inputs)
    {
      if (Ability == AbilityTag) return Input;
    }

    return FGameplayTag();
  }

  void Reset()
  {
    for (auto& Item : Inputs)
    {
      Item.Value = FGameplayTag();
    }
  }
};

USTRUCT(BlueprintType)
struct FCombatManagerSave
{
  GENERATED_BODY()
  UPROPERTY(BlueprintReadWrite)
  int32 CombatsCount = 0;

  void Reset()
  {
    CombatsCount = 0;
  }
};

USTRUCT(BlueprintType)
struct FLocationManagerSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  int32 Index = -1;

  void Reset()
  {
    Index = -1;
  }
};

USTRUCT(BlueprintType)
struct FRewardManagerSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FGameplayTagContainer BlockedRewards = FGameplayTagContainer();
  FGameplayTag NextRewardTag = FGameplayTag();
  TArray<FGameplayTag> RewardBag = TArray<FGameplayTag>();

  void Reset()
  {
    BlockedRewards = FGameplayTagContainer();
    NextRewardTag = FGameplayTag();
    RewardBag.Empty();
  }
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
  UAuraSaveGame();

  virtual void PostInitProperties() override;

  void OnPlayerDeath();

  bool bJustLoaded = false;
  
  UPROPERTY(BlueprintReadWrite)
  FSaveInfo SaveInfo = FSaveInfo();
  UPROPERTY(BlueprintReadWrite)
  FPlayerStateSave PlayerState = FPlayerStateSave();
  UPROPERTY(BlueprintReadWrite)
  FAttributeSetSave AttributeSet = FAttributeSetSave();
  UPROPERTY(BlueprintReadWrite)
  FAbilityManagerSave AbilityManager = FAbilityManagerSave();
  UPROPERTY(BlueprintReadWrite)
  FAbilityInputSave AbilityInput = FAbilityInputSave();
  UPROPERTY(BlueprintReadWrite)
  FCombatManagerSave CombatManager = FCombatManagerSave();
  UPROPERTY(BlueprintReadWrite)
  FLocationManagerSave LocationManager = FLocationManagerSave();
  UPROPERTY(BlueprintReadWrite)
  FRewardManagerSave RewardManager = FRewardManagerSave();
  
protected:

private:
  void InitPlayerState();
  void InitAttributeSet();
  void InitAbilityInputs();
};
