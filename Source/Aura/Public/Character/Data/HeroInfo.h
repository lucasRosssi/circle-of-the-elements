// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/CharacterName.h"
#include "HeroInfo.generated.h"

class AAuraHero;

USTRUCT(BlueprintType)
struct FHeroData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  ECharacterName HeroName = ECharacterName::Undefined;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<AAuraHero> HeroClass;

  bool IsValid() const { return HeroName != ECharacterName::Undefined; }
};

/**
 * 
 */
UCLASS()
class AURA_API UHeroInfo : public UDataAsset
{
	GENERATED_BODY()

public:

  FHeroData GetHeroData(ECharacterName HeroName);
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TMap<ECharacterName, FHeroData> HeroData;
};
