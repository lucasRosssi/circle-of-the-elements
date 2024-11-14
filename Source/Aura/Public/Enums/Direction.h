#pragma once

UENUM(BlueprintType)
enum class EDirection : uint8
{
	North UMETA(DisplayName = "North"),
	East UMETA(DisplayName = "East"),
	South UMETA(DisplayName = "South"),
  West UMETA(DisplayName = "West"),
  None UMETA(DisplayName = "None"),

	MAX UMETA(Hidden)
};
