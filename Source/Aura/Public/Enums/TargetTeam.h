#pragma once

UENUM(BlueprintType)
enum class ETargetTeam : uint8
{
	ETT_Both UMETA(DisplayName = "Both"),
	ETT_Friends UMETA(DisplayName = "Friends"),
	ETT_Enemies UMETA(DisplayName = "Enemies"),
	ETT_Self UMETA(DisplayName = "Self"),

	ETT_MAX UMETA(DisplayName = "DefaultMax")
};
