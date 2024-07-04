#pragma once

UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	Undefined UMETA(DisplayName = "Undefined"),
	Aura UMETA(DisplayName = "Aura"),
	Vilkar UMETA(DisplayName = "Vilkar"),

	MAX UMETA(Hidden)
};
