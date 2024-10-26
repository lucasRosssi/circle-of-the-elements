// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/Data/HeroInfo.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/ManagerInterface.h"
#include "AuraGameModeBase.generated.h"

class AAuraHUD;
class UAuraGameInstance;

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
