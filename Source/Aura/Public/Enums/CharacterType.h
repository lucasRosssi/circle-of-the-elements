#pragma once

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	ECT_Player UMETA(DisplayName = "Player"),
	ECT_Companion UMETA(DisplayName = "Companion"),
	ECT_Regular UMETA(DisplayName = "Regular"),
	ECT_MiniBoss UMETA(DisplayName = "Mini-Boss"),
	ECT_Boss UMETA(DisplayName = "Boss"),
	ECT_Minion UMETA(DisplayName = "Minion"),

	ECT_MAX UMETA(DisplayName = "DefaultMax")
};
