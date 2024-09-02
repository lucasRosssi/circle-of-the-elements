#pragma once

UENUM(BlueprintType)
enum class EMenuBlurMode : uint8
{
	None UMETA(DisplayName = "None"),
	Partial UMETA(DisplayName = "Partial"),
	Full UMETA(DisplayName = "Full"),

	MAX UMETA(Hidden)
};
