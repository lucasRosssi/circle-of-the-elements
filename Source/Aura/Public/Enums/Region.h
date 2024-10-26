#pragma once

UENUM(BlueprintType)
enum class ERegion : uint8
{
	Undefined UMETA(DisplayName = "Undefined"),
	CrystalCave UMETA(DisplayName = "Crystal Cave"),
  Village UMETA(DisplayName = "Village"),

	MAX UMETA(Hidden)
};
