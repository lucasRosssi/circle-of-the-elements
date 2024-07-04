#pragma once

UENUM(BlueprintType)
enum class EStatusEffectPosition : uint8
{
	Top UMETA(DisplayName = "Top"),
	Center UMETA(DisplayName = "Center"),
	Bottom UMETA(DisplayName = "Bottom"),

	MAX UMETA(Hidden)
};
