#pragma once

UENUM(BlueprintType)
enum class EStatusEffectPosition : uint8
{
	ESEP_Top UMETA(DisplayName = "Top"),
	ESEP_Center UMETA(DisplayName = "Center"),
	ESEP_Bottom UMETA(DisplayName = "Bottom"),

	ESEP_MAX UMETA(DisplayName = "DefaultMax")
};
