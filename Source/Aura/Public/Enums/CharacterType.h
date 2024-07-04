#pragma once

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player UMETA(DisplayName = "Player"),
	Companion UMETA(DisplayName = "Companion"),
	Regular UMETA(DisplayName = "Regular"),
	MiniBoss UMETA(DisplayName = "Mini-Boss"),
	Boss UMETA(DisplayName = "Boss"),
	Minion UMETA(DisplayName = "Minion"),

	MAX UMETA(Hidden)
};
