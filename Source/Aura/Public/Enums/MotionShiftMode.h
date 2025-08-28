#pragma once

UENUM(BlueprintType)
enum class EMotionShiftMode : uint8
{
  // Shifts the initial direction of the movement (left, right, clockwise...)
  Direction     UMETA(DisplayName = "Direction"),
  // Shifts in which phase the motion starts
  Phase         UMETA(DisplayName = "Phase"),
  // Shifts the frequency or duration of the motion
  Speed         UMETA(DisplayName = "Speed"),
  // Shifts the amplitude over time
  Growth         UMETA(DisplayName = "Growth"),
  // Bezier only: shifts the control points rotations
  ControlPoint  UMETA(DisplayName = "ControlPoint"),

  MAX           UMETA(Hidden)
};
