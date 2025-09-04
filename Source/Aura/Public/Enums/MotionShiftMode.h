#pragma once

UENUM(BlueprintType)
enum class EMotionShiftMode : uint8
{
  // Shifts the initial direction of the movement (left, right, clockwise...)
  Direction         UMETA(DisplayName = "Direction"),
  // Shifts in which phase the motion starts
  Phase             UMETA(DisplayName = "Phase"),
  // Shifts the frequency, duration or velocity of the motion
  Speed             UMETA(DisplayName = "Speed"),
  // Shifts the amplitude over time
  Growth            UMETA(DisplayName = "Growth"),
  // Shifts the speed factor of yo-yo's return phase
  YoYoReturnSpeed   UMETA(DisplayName = "Yo-yo Return Speed"),
  // Bezier only: shifts the control points rotations
  ControlPoint      UMETA(DisplayName = "ControlPoint"),

  MAX               UMETA(Hidden)
};
