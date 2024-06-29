// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1, ClampMax=20))
	int32 Level = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0))
	int32 XPRequirement = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=10))
	int32 AttributePoints = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=10))
	int32 SkillPoints = 1;
};


/**
 * 
 */
UCLASS()
class AURA_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="LevelInformation",
		meta=(TitleProperty="{Level}")
	)
	TArray<FLevelUpInfo> LevelInformation;

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 FindLevelByXP(int32 XP) const;

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 GetLevelUpAttributePoints(int32 OldLevel, int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category="LevelInformation")
	int32 GetLevelUpSkillPoints(int32 OldLevel, int32 NewLevel);
};
