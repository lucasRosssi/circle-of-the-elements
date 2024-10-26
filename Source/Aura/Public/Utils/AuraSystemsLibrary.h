// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraSystemsLibrary.generated.h"

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
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static UAbilityInfo* GetAbilitiesInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static UCharacterInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static UHeroInfo* GetHeroInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static URegionInfo* GetRegionInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static URewardsInfo* GetRewardsInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static UStatusEffectInfo* GetStatusEffectInfo(const UObject* WorldContextObject);
  UFUNCTION(BlueprintPure, Category="Aura Systems|Info", meta=(DefaultsToSelf="WorldContextObject", HidePin="WorldContextObject"))
  static UUpgradeInfo* GetUpgradeInfo(const UObject* WorldContextObject);
  // END Data Assets

  // Managers
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Hero",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UAbilityManager* GetAbilityManager(const UObject* WorldContextObject);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Managers|Camera",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static ACameraManager* GetCameraManager(const UObject* WorldContextObject);
  UFUNCTION(
    BlueprintPure,
    Category="Aura Systems|Managers|Hero",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static UUpgradeManager* GetUpgradeManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static ULocationManager* GetLocationManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Combat",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UCombatManager* GetCombatManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static URewardManager* GetRewardManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|UI",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UUIManager* GetUIManager(const UObject* WorldContextObject, int32 PlayerIndex);
  // END Managers
  
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
    BlueprintCallable,
    Category="Aura Systems|Save",
    meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
    )
  static void SaveGameData(
    const UObject* WorldContextObject,
    const FString& PlayerName,
    int32 SlotIndex
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

private:
  static AGameModeBase* GetManagerInterfaceGameMode(const UObject* WorldContextObject);
};
