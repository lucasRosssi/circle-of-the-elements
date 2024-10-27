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
};

USTRUCT(BlueprintType)
struct FAttributeSetSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, float> Attributes = TMap<FGameplayTag, float>();
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
  
  UPROPERTY(BlueprintReadWrite)
  FSaveInfo SaveInfo = FSaveInfo();
  UPROPERTY(BlueprintReadWrite)
  FPlayerStateSave PlayerState = FPlayerStateSave();
  UPROPERTY(BlueprintReadWrite)
  FAttributeSetSave AttributeSet = FAttributeSetSave();
  UPROPERTY(BlueprintReadWrite)
  FAbilityManagerSave AbilityManager = FAbilityManagerSave();

  UPROPERTY(BlueprintReadWrite)
  int32 LocationIndex = -1;
  
protected:

private:
  
};
