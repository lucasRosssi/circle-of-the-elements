#pragma once

UENUM(BlueprintType)
enum class EMeleeHitMode : uint8
{
	// Melee hit scan will occur with a Sphere shaped collision (instant scan)
	Sphere UMETA(DisplayName = "Sphere"),
	// Melee hit scan will occur with a Box shaped collision (instant scan)
	Box UMETA(DisplayName = "Box"),
	// Melee hit scan will occur through the Weapon Box collision (scan window)
	Weapon UMETA(DisplayName = "Weapon"),

	MAX UMETA(Hidden)
};
