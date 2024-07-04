#pragma once

UENUM(BlueprintType)
enum class ETargetTeam : uint8
{
	Both UMETA(DisplayName = "Both"),
	Friends UMETA(DisplayName = "Friends"),
	Enemies UMETA(DisplayName = "Enemies"),
	Self UMETA(DisplayName = "Self"),

	MAX UMETA(Hidden)
};
