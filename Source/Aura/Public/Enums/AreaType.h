#pragma once

UENUM(BlueprintType)
enum class EAreaType : uint8
{
	Arena UMETA(DisplayName = "Arena"),
	Resource UMETA(DisplayName = "Resource"),
  Entrance UMETA(DisplayName = "Entrance"),
	Exit UMETA(DisplayName = "Exit"),

	MAX UMETA(Hidden)
};
