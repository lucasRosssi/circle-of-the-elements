// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.generated.h"

class UAuraSaveGame;

UCLASS( ClassGroup="Managers", meta=(BlueprintSpawnableComponent) )
class AURA_API UAuraSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAuraSystemComponent();

protected:
  UAuraSaveGame* GetSaveGame();

  UPROPERTY()
  UAuraSaveGame* SaveGame = nullptr;

private:
		
};
