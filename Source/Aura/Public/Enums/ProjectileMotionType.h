#pragma once

UENUM(BlueprintType)
enum class EProjectileMotionType : uint8
{
  Default   UMETA(DisplayName = "Default"),
  Helix     UMETA(DisplayName = "Helix"),
  Snake     UMETA(DisplayName = "Snake"),
  Noise     UMETA(DisplayName = "Noise"),
  Pulse     UMETA(DisplayName = "Pulse"),
  ZigZag    UMETA(DisplayName = "ZigZag"),
  YoYo      UMETA(DisplayName = "Yo-Yo"),
  Bezier    UMETA(DisplayName = "Bezier Curve"),

  MAX       UMETA(Hidden)
};
