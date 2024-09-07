#pragma once

UENUM(BlueprintType)
enum class EProjectileHomingMode : uint8
{
	// All projectiles will follow the same target
	OneTarget UMETA(DisplayName = "One Target"),
	// Projectiles will try to have different targets when possible
	DifferentTargets UMETA(DisplayName = "Different Targets"),
	// Projectiles will follow random targets.
	RandomTargets UMETA(DisplayName = "Random Targets"),

	MAX UMETA(Hidden)
};
