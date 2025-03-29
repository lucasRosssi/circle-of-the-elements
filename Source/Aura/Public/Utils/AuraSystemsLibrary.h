// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraSystemsLibrary.generated.h"

class FOnCombatFinished;
class UMatchManager;
enum class ECharacterType : uint8;
class ULevelInfo;
class AAuraPlayerState;
struct FSaveInfo;
class ACameraManager;
struct FHeroData;
class UHeroInfo;
class UUpgradeInfo;
enum class ECharacterName : uint8;
class URewardsInfo;
class URegionInfo;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;
class UUpgradeManager;
class AAuraHUD;
class UUIManager;
class UAbilityManager;
class ULocationManager;
class URewardManager;
class UCombatManager;
class UAuraSaveGame;
/**
 * 
 */
UCLASS()
class AURA_API UAuraSystemsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
  // Data Assets
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static UAbilityInfo* GetAbilitiesInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static UCharacterInfo* GetCharacterClassInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static UHeroInfo* GetHeroInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static URegionInfo* GetRegionInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static URewardsInfo* GetRewardsInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static UStatusEffectInfo* GetStatusEffectInfo(UPARAM(DisplayName="Target")const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static UUpgradeInfo* GetUpgradeInfo(UPARAM(DisplayName="Target") const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultToSelf="WorldContextObject"))
  static ULevelInfo* GetLevelInfo(UPARAM(DisplayName="Target") const UObject* WorldContextObject);
  // END Data Assets

  // Managers
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Hero",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UAbilityManager* GetAbilityManager(const UObject* WorldContextObject);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Camera",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static ACameraManager* GetCameraManager(const UObject* WorldContextObject);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Hero",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static UUpgradeManager* GetUpgradeManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static ULocationManager* GetLocationManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Combat",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UCombatManager* GetCombatManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static URewardManager* GetRewardManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|UI",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UUIManager* GetUIManager(const UObject* WorldContextObject, int32 PlayerIndex);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Match",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static UMatchManager* GetMatchManager(const UObject* WorldContextObject);
  // END Managers
  // Managers Delegates
  static FOnCombatFinished& GetOnCombatFinishedDelegate(const UObject* WorldContextObject);
  // END Managers Delegates
  
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|UI",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static AAuraHUD* GetAuraHUD(const UObject* WorldContextObject, int32 PlayerIndex);

  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Hero",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static FHeroData GetCurrentHeroData(const UObject* WorldContextObject);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Hero",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static AAuraPlayerState* GetAuraPlayerState(const UObject* WorldContextObject);

  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void SaveGameData(
    const UObject* WorldContextObject,
    const FSaveInfo& SaveData
    );
  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void SaveCurrentGame(
    const UObject* WorldContextObject
    );
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static UAuraSaveGame* LoadGameData(
    const UObject* WorldContextObject,
    int32 SlotIndex
    );
  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void DeleteGameData(
    const UObject* WorldContextObject,
    int32 SlotIndex
    );
  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void LoadGameAndPlay(
    const UObject* WorldContextObject,
    int32 SlotIndex
    );
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static UAuraSaveGame* GetCurrentSaveGameObject(const UObject* WorldContextObject);

  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Leveling",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void StackXP(const ACharacter* RewardedCharacter, ECharacterType CharacterType, int32 Level);
  
  UFUNCTION(
    BlueprintCallable,
    Category="Aura Systems|Location",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void BackToHome(const UObject* WorldContextObject);

private:
  static AGameModeBase* GetManagerInterfaceGameMode(const UObject* WorldContextObject);
};
