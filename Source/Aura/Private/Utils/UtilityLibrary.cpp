// Copyright Lucas Rossi


#include "Utils/UtilityLibrary.h"

float UUtilityLibrary::GetDistance(const FVector& V1, const FVector& V2)
{
	return (V1 - V2).Length() / 2;
}

FVector UUtilityLibrary::GetMiddlePoint(const FVector& V1, const FVector& V2)
{
	return (V1 + V2) / 2;
}
