#pragma once

UENUM(BlueprintType)
enum class EDifficultyClass : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Normal UMETA(DisplayName = "Normal"),
	Hard UMETA(DisplayName = "Hard"),
	Extreme UMETA(DisplayName = "Extreme"),
	Special UMETA(DisplayName = "Special"),

	MAX UMETA(Hidden)
};
