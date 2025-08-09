// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

struct FGameplayTag;
struct FScalableFloat;
/**
 * 
 */
UCLASS()
class AURA_API UUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Vectors")
	static float GetDistance(const FVector& V1, const FVector& V2);

	UFUNCTION(BlueprintPure, Category="Vectors")
	static FVector GetMiddlePoint(const FVector& V1, const FVector& V2);

  UFUNCTION(BlueprintPure, Category="Numbers|Scalable")
  static float GetScalableFloatAtLevel(const FScalableFloat& ScalableFloat, int32 Level);
  UFUNCTION(BlueprintPure, Category="Numbers|Scalable")
  static int32 GetScalableIntegerAtLevel(const FScalableFloat& ScalableFloat, int32 Level);

  UFUNCTION(BlueprintPure, Category="Probability")
  static FGameplayTag PickRandomWeightedTagNormalized(const TMap<FGameplayTag, float>& WeightedTags);
};
