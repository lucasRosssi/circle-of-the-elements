#pragma once

UENUM(BlueprintType)
enum class EGatePosition : uint8
{
	BottomLeft UMETA(DisplayName = "Bottom Left"),
	BottomRight UMETA(DisplayName = "Bottom Right"),
	TopLeft UMETA(DisplayName = "Top Left"),
	TopRight UMETA(DisplayName = "Top Right"),

	MAX UMETA(Hidden)
};
