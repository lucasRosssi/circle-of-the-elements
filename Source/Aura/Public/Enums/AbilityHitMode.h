#pragma once

UENUM(BlueprintType)
enum class EAbilityHitMode : uint8
{
	// Ability has default behavior and usually finishes in one hit
	Default UMETA(DisplayName = "Default"),
	// Ability bounces, chains or leaps to another target after first hit
	Bounce UMETA(DisplayName = "Bounce"),
	// Ability penetrates first hit and can hit more targets in the way before finishing
	Penetration UMETA(DisplayName = "Penetration"),

	MAX UMETA(Hidden)
};
