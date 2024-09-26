// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RewardsInfo.generated.h"

class ALocationReward;

USTRUCT(BlueprintType)
struct FRewardInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ALocationReward> RewardClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1))
	int32 Amount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Limit = MAX_int32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DropRateWeight = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(MultiLine))
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BackgroundMaterial = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API URewardsInfo : public UDataAsset
{
	GENERATED_BODY()

	URewardsInfo();
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	FRewardInfo GetRewardInfo(const FGameplayTag& RewardTag);

	FRewardInfo GetRandomizedReward();

	void FillRewardBag(
	  TArray<FGameplayTag>& OutBag,
	  const FGameplayTagContainer& BlockedRewards = FGameplayTagContainer()
	  );
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Resources", ForceInlineRow))
	TMap<FGameplayTag, FRewardInfo> Rewards;

	UPROPERTY(VisibleAnywhere)
	float DropSum = 0.f;
};
