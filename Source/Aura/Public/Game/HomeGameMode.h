// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraGameModeBase.h"
#include "HomeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AHomeGameMode : public AAuraGameModeBase
{
	GENERATED_BODY()

public:
  AHomeGameMode();
  
  // Manager Interface
  virtual UAbilityManager* GetAbilityManager_Implementation() const override { return AbilityManager; }
  virtual UUpgradeManager* GetUpgradeManager_Implementation() const override { return UpgradeManager; }
  // END Manager Interface

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Character|Ability")
  TObjectPtr<UAbilityManager> AbilityManager;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Managers|Character|Upgrade")
  TObjectPtr<UUpgradeManager> UpgradeManager;
};
