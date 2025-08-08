// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EnemiesInfo.generated.h"

class AAuraEnemy;

USTRUCT(BlueprintType)
struct FEnemyInfo
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TSubclassOf<AAuraEnemy> EnemyClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, UIMin=1))
  float DifficultyCost = 1.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UEnemiesInfo : public UDataAsset
{
  GENERATED_BODY()

public:
  FEnemyInfo GetEnemyInfo(const FGameplayTag& EnemyTag) const;
	
protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Enemies", ForceInlineRow))
  TMap<FGameplayTag, FEnemyInfo> Enemies;
};
