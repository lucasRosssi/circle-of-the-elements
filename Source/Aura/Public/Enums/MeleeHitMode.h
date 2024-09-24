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
  // Melee hit scan will occur with a Cone shaped collision (instant scan)
  Cone UMETA(DisplayName = "Cone"),
  // Melee hit scan will occur through multiple Line Traces (scan window)
  LineTraces UMETA(DisplayName = "Line Traces"),

	MAX UMETA(Hidden)
};
