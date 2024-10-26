// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AuraSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
  UPROPERTY()
  FString SlotName = FString();
  UPROPERTY()
  int32 SlotIndex = 0;
  UPROPERTY(BlueprintReadOnly)
  FString PlayerName = FString("Default Name");
  
protected:

private:
  
};
