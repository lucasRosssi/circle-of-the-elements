// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Equipment/Rune.h"
#include "AbilitySystem/Equipment/Spirit.h"
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
  int32 XP = 0;
  UPROPERTY(BlueprintReadWrite)
  int32 AttributePoints = 0;
  UPROPERTY(BlueprintReadWrite)
  int32 SkillPoints = 0;
  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, int32> Resources = TMap<FGameplayTag, int32>();
  UPROPERTY(BlueprintReadWrite)
  TArray<FSpiritInfo> SpiritsInventory = TArray<FSpiritInfo>();
  UPROPERTY(BlueprintReadWrite)
  TArray<FRuneInfo> RunesInventory = TArray<FRuneInfo>();
  UPROPERTY(BlueprintReadWrite)
  TMap<int32, FGuid> Loadout = TMap<int32, FGuid>();

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
  UPROPERTY(BlueprintReadWrite)
  TArray<int32> SelectedLocationsIndex;

  void Reset()
  {
    Index = -1;
  }
};

USTRUCT(BlueprintType)
struct FUpgradeManagerSave
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  TMap<FGameplayTag, int32> AcquiredUpgrades = TMap<FGameplayTag, int32>();
  UPROPERTY(BlueprintReadWrite)
  FGameplayTagContainer EquippedUpgrades = FGameplayTagContainer();
  UPROPERTY(BlueprintReadWrite)
  FGameplayTagContainer BlockedUpgrades = FGameplayTagContainer();
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
  FCombatManagerSave CombatManager = FCombatManagerSave();
  UPROPERTY(BlueprintReadWrite)
  FLocationManagerSave LocationManager = FLocationManagerSave();
  UPROPERTY(BlueprintReadWrite)
  FUpgradeManagerSave UpgradeManager = FUpgradeManagerSave();

  UPROPERTY(BlueprintReadWrite)
  float HeroHealth = 100.f;

protected:

private:
  void InitPlayerState();
  void InitAttributeSet();
};
