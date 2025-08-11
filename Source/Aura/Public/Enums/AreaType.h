#pragma once

UENUM(BlueprintType)
enum class EAreaType : uint8
{
  Entrance UMETA(DisplayName = "Entrance"),
	DefaultArena UMETA(DisplayName = "DefaultArena"),
	SpiritArena UMETA(DisplayName = "SpiritArena"),
	BossArena UMETA(DisplayName = "BossArena"),
	RewardArea UMETA(DisplayName = "RewardArea"),
	SpecialArea UMETA(DisplayName = "SpecialArea"),
	Exit UMETA(DisplayName = "Exit"),

	MAX UMETA(Hidden)
};
