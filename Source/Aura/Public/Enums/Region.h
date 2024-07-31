#pragma once

UENUM(BlueprintType)
enum class ERegion : uint8
{
	Undefined UMETA(DisplayName = "Undefined"),
	CrystalCave UMETA(DisplayName = "Crystal Cave"),

	MAX UMETA(Hidden)
};
