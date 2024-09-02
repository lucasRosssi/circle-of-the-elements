// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraSystemsLibrary.generated.h"

class AAuraHUD;
class UUIManager;
class UAbilityManager;
class ULocationManager;
class URewardManager;
class UEncounterManager;
/**
 * 
 */
UCLASS()
class AURA_API UAuraSystemsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Ability",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UAbilityManager* GetAbilityManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static ULocationManager* GetLocationManager(const UObject* WorldContextObject);
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|Managers|Location",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static UEncounterManager* GetEncounterManager(const UObject* WorldContextObject);
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
	UFUNCTION(
		BlueprintPure,
		Category="Aura Systems|UI",
		meta=(HidePin="WorldContextObject", DefaultToSelf="WorldContextObject")
		)
	static AAuraHUD* GetAuraHUD(const UObject* WorldContextObject, int32 PlayerIndex);

};
