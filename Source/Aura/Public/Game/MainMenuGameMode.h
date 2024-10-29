// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraGameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMainMenuGameMode : public AAuraGameModeBase
{
	GENERATED_BODY()

public:

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sound")
  TObjectPtr<USoundBase> MainMenuMusic;
  
private:
  
};
