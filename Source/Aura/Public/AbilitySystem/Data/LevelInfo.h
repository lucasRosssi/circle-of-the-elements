// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

enum class ECharacterType : uint8;

/**
 * 
 */
UCLASS()
class AURA_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FScalableFloat XPRequirement;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FScalableFloat AttributePoints;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FScalableFloat SkillPoints;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FScalableFloat XPReward;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TMap<ECharacterType, float> XPRewardMultiplier;

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 FindLevelByXP(int32 XP) const;

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 GetLevelUpAttributePoints(int32 OldLevel, int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 GetLevelUpSkillPoints(int32 OldLevel, int32 NewLevel);

  UFUNCTION(BlueprintCallable, Category="LevelInformation")
  int32 GetXPRewardForTypeAndLevel(ECharacterType CharacterType, int32 Level);
};
