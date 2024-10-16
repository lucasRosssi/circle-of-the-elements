#pragma once

UENUM(BlueprintType)
enum class ECameraState : uint8
{
	Default UMETA(DisplayName = "Default"),
	PlayerDeath UMETA(DisplayName = "PlayerDeath"),
  BossDeath UMETA(DisplayName = "BossDeath"),
  SecondWind UMETA(DisplayName = "SecondWind"),

	MAX UMETA(Hidden)
};
