// Copyright Lucas Rossi


#include "Utils/UtilityLibrary.h"

#include "ScalableFloat.h"

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
