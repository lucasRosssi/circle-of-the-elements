#pragma once

UENUM(BlueprintType)
enum class ESaveSlotState : uint8
{
	Empty UMETA(DisplayName = "Empty"),
	Naming UMETA(DisplayName = "Naming"),
  Occupied UMETA(DisplayName = "Occupied"),

	MAX UMETA(Hidden)
};
