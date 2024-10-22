// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/Data/HeroInfo.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/ManagerInterface.h"
#include "AuraGameModeBase.generated.h"

class AAuraHUD;
class UAuraGameInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastEnemyKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRewardTaken);

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase, public IManagerInterface
{
	GENERATED_BODY()

public:
	AAuraGameModeBase();

  UAuraGameInstance* GetAuraGameInstance();
  AAuraHUD* GetAuraHUD(int32 PlayerIndex);
  FHeroData GetCurrentHeroData();

  UFUNCTION(BlueprintCallable)
  void SpawnAndPossessSelectedHero();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player|Hero")
  FHeroData CurrentHeroData;

protected:
  TMap<int32, AAuraHUD*> AuraHUDs;
};
