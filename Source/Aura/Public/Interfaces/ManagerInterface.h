// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ManagerInterface.generated.h"

class UMatchManager;
class UUpgradeManager;
class URewardManager;
class ULocationManager;
class UCombatManager;
class UAbilityManager;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAbilityManager* GetAbilityManager() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  UCombatManager* GetCombatManager() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  ULocationManager* GetLocationManager() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  URewardManager* GetRewardManager() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  UUpgradeManager* GetUpgradeManager() const;
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  UMatchManager* GetMatchManager() const;
};
