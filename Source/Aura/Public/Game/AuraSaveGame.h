// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Enums/CharacterName.h"
#include "Enums/Region.h"
#include "AuraSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveInfo
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FString SlotName = FString();
  UPROPERTY(BlueprintReadWrite)
  int32 SlotIndex = -1;
  UPROPERTY(BlueprintReadWrite)
  FString PlayerName = FString("Player");
  UPROPERTY(BlueprintReadWrite)
  ECharacterName HeroName = ECharacterName::Undefined;
  UPROPERTY(BlueprintReadWrite)
  ERegion RegionName = ERegion::Undefined;

  bool IsValid() const
  {
    return !SlotName.IsEmpty() && SlotIndex >= 0 && HeroName != ECharacterName::Undefined;
  }
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
  UPROPERTY(BlueprintReadWrite)
  FSaveInfo SaveInfo = FSaveInfo();

  UPROPERTY(BlueprintReadWrite)
  int32 LocationIndex = -1;
  
protected:

private:
  
};
