#pragma once

UENUM(BlueprintType)
enum class ECombatMode : uint8
{
	Defined UMETA(DisplayName = "Defined"),
	DifficultyPoints UMETA(DisplayName = "DifficultyPoints"),

	MAX UMETA(Hidden)
};
