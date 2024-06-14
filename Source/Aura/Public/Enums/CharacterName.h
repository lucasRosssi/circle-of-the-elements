#pragma once

UENUM(BlueprintType)
enum class ECharacterName : uint8
{
	ECN_Undefined UMETA(DisplayName = "Undefined"),
	ECN_Aura UMETA(DisplayName = "Aura"),
	ECN_Vilkar UMETA(DisplayName = "Vilkar"),

	ECN_MAX UMETA(DisplayName = "DefaultMax")
};
