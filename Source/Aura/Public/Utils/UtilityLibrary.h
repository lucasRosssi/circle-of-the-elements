// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

enum class ECardinalDirection : uint8;
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

  UFUNCTION(BlueprintPure, Category="Direction")
  static ECardinalDirection GetOppositeDirection(ECardinalDirection Direction);
  UFUNCTION(BlueprintPure, Category="Direction")
  static FIntPoint GetCoordinateOffsetFromDirection(ECardinalDirection Direction);
  UFUNCTION(BlueprintPure, Category="Direction")
  static ECardinalDirection GetDirectionFromCoordinateOffset(const FIntPoint& Coordinate);
  UFUNCTION(BlueprintPure, Category="Direction")
  static TArray<FIntPoint> GetAdjacentCoordinates(const FIntPoint& Coordinate);

  template <typename TEnum>
  UFUNCTION(BlueprintPure, Category="Enums")
  static FString EnumToString(TEnum EnumValue)
  {
    static_assert(TIsEnumClass<TEnum>::Value, "EnumToString requires a strongly typed enum (enum class).");

    const UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr)
    {
      return TEXT("InvalidEnum");
    }

    const int64 EnumValueInt = static_cast<int64>(EnumValue);

    const FText DisplayName = EnumPtr->GetDisplayNameTextByValue(EnumValueInt);

    if (DisplayName.IsEmpty() || DisplayName.ToString() == "None")
    {
      return EnumPtr->GetNameStringByValue(EnumValueInt);
    }

    return DisplayName.ToString();
  }
};
