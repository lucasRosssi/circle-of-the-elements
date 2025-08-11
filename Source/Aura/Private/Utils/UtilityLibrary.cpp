// Copyright Lucas Rossi


#include "Utils/UtilityLibrary.h"

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Aura/AuraMacros.h"
#include "Enums/CardinalDirection.h"

float UUtilityLibrary::GetDistance(const FVector& V1, const FVector& V2)
{
	return (V1 - V2).Size();
}

FVector UUtilityLibrary::GetMiddlePoint(const FVector& V1, const FVector& V2)
{
	return (V1 + V2) / 2;
}

float UUtilityLibrary::GetScalableFloatAtLevel(const FScalableFloat& ScalableFloat, int32 Level)
{
  return ScalableFloat.GetValueAtLevel(Level);
}

int32 UUtilityLibrary::GetScalableIntegerAtLevel(const FScalableFloat& ScalableFloat, int32 Level)
{
  return ScalableFloat.AsInteger(Level);
}

FGameplayTag UUtilityLibrary::PickRandomWeightedTagNormalized(const TMap<FGameplayTag, float>& WeightedTags)
{
  TArray<FGameplayTag> Tags;
  TArray<float> Weights;
  float TotalWeight = 0.f;

  for (const auto& Elem : WeightedTags)
  {
    if (Elem.Value > 0.f)
    {
      Tags.Add(Elem.Key);
      Weights.Add(Elem.Value);
      TotalWeight += Elem.Value;
    }
  }

  if (Tags.Num() == 0 || TotalWeight <= 0.f)
  {
    // Handle error
    return FGameplayTag(); // or fallback tag
  }

  // Normalize weights
  for (float& Weight : Weights)
  {
    Weight /= TotalWeight;
  }

  // Weighted random pick
  const float Roll = FMath::FRand(); // [0.0, 1.0)
  float Accum = 0.f;

  for (int32 i = 0; i < Tags.Num(); ++i)
  {
    Accum += Weights[i];
    if (Roll <= Accum)
      return Tags[i];
  }

  return Tags.Last(); // fallback in case of float rounding errors
}

ECardinalDirection UUtilityLibrary::GetOppositeDirection(ECardinalDirection Direction)
{
  switch (Direction)
  {
  case ECardinalDirection::North: return ECardinalDirection::South;
  case ECardinalDirection::South: return ECardinalDirection::North;
  case ECardinalDirection::East:  return ECardinalDirection::West;
  case ECardinalDirection::West:  return ECardinalDirection::East;
  default: return Direction;
  }
}

FIntPoint UUtilityLibrary::GetCoordinateOffsetFromDirection(ECardinalDirection Direction)
{
  const TMap<ECardinalDirection, FIntPoint> DirectionOffsets = {
    {ECardinalDirection::North, FIntPoint(0, 1)},
    {ECardinalDirection::East, FIntPoint(1, 0)},
    {ECardinalDirection::South, FIntPoint(0, -1)},
    {ECardinalDirection::West, FIntPoint(-1, 0)},
  };

  return DirectionOffsets[Direction];
}
ECardinalDirection UUtilityLibrary::GetDirectionFromCoordinateOffset(const FIntPoint& Coordinate)
{
  GUARD(FMath::Abs(Coordinate.X) <= 1 && FMath::Abs(Coordinate.Y) <= 1, ECardinalDirection::North, TEXT("Invalid coordinate offset: %s"), *Coordinate.ToString())
  
  const TMap<FIntPoint, ECardinalDirection> OffsetDirections = {
    { FIntPoint(0, 1), ECardinalDirection::North },
    { FIntPoint(1, 0), ECardinalDirection::East },
    { FIntPoint(0, -1), ECardinalDirection::South },
    { FIntPoint(-1, 0), ECardinalDirection::West },
  };

  return OffsetDirections[Coordinate];
}

TArray<FIntPoint> UUtilityLibrary::GetAdjacentCoordinates(const FIntPoint& Coordinate)
{
  return TArray({
    FIntPoint(Coordinate.X, Coordinate.Y + 1),
    FIntPoint(Coordinate.X + 1, Coordinate.Y),
    FIntPoint(Coordinate.X, Coordinate.Y - 1),
    FIntPoint(Coordinate.X - 1, Coordinate.Y)
  });
}

