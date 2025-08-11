#pragma once

UENUM(BlueprintType)
enum class ECardinalDirection : uint8
{
	North UMETA(DisplayName = "North"),
	East UMETA(DisplayName = "East"),
  South UMETA(DisplayName = "South"),
	West UMETA(DisplayName = "West"),

	MAX UMETA(Hidden)
};
