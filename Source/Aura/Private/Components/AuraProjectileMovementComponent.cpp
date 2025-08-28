// Copyright Lucas Rossi


#include "Components/AuraProjectileMovementComponent.h"


UAuraProjectileMovementComponent::UAuraProjectileMovementComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UAuraProjectileMovementComponent::BeginPlay()
{
  Super::BeginPlay();

  StartVelocity = Velocity;
  if (UpdatedComponent)
  {
    StartLocation = UpdatedComponent->GetComponentLocation();
    StartTransform = UpdatedComponent->GetComponentTransform();
  }

  if (!bDefinedDirections && MotionShiftModes.Contains(EMotionShiftMode::Direction))
  {
    DirectionMultiplier = FMath::RandBool() ? 1.f : -1.f;
  }
  // To ensure those values if setting it from outside
  DirectionMultiplier = FMath::Clamp(DirectionMultiplier, -1.f, 1.f);

  if (MotionShiftModes.Contains(EMotionShiftMode::Phase))
  {
    RandomPhaseShift = FMath::FRandRange(0.f, 2.f * PI);
  }

  if (MotionShiftModes.Contains(EMotionShiftMode::Speed))
  {
    const float RandomSpeedScale = FMath::FRandRange(1.f - MaxSpeedShiftAmplitude, 1.f + MaxSpeedShiftAmplitude);

    switch (ActiveMotion)
    {
    case EProjectileMotionType::Bezier:
      {
        EffectiveBezierDuration = BezierDuration / RandomSpeedScale;
        break;
      }
    case EProjectileMotionType::Noise:
      {
        EffectiveNoiseInterval = NoiseInterval / RandomSpeedScale;
        break;
      }
    default:
      {
        EffectiveFrequency = Frequency * RandomSpeedScale;
      }
    }
  }
  else
  {
    EffectiveBezierDuration = BezierDuration;
    EffectiveFrequency = Frequency;
  }

  if (MotionShiftModes.Contains(EMotionShiftMode::ControlPoint))
  {
    const float Angle = FMath::FRandRange(-MaxRandomBezierRotation / 2, MaxRandomBezierRotation / 2);
    RandomBezierRotation = FRotator(0.f, Angle, 0.f);
  }
}

void UAuraProjectileMovementComponent::TickComponent(
  float DeltaTime,
  ELevelTick TickType,
  FActorComponentTickFunction* ThisTickFunction
)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (
    ActiveMotion == EProjectileMotionType::Default ||
    bIsHomingProjectile ||
    !UpdatedComponent ||
    ShouldSkipUpdate(DeltaTime)
  )
    return;

  ElapsedTime += DeltaTime;

  FVector TotalOffset;

  // --- Bezier full override ---
  if (ActiveMotion == EProjectileMotionType::Bezier)
  {
    float T = ElapsedTime / EffectiveBezierDuration;

    if (MotionShiftModes.Contains(EMotionShiftMode::Phase))
    {
      T += RandomPhaseShift / (2.f * PI);
    }

    T = FMath::Clamp(T, 0.f, 1.f);

    const FVector NewPos = ComputeBezierPosition(T);
    const FVector PrevPos = ComputeBezierPosition(FMath::Clamp(T - DeltaTime / BezierDuration, 0.f, 1.f));

    TotalOffset = NewPos - PrevPos;

    UpdatedComponent->AddWorldOffset(TotalOffset, false);
    Velocity = TotalOffset / DeltaTime;
    return;
  }

  // --- Yo-yo full override ---
  if (ActiveMotion == EProjectileMotionType::YoYo)
  {
    const FVector NewPos = ComputeYoYoPosition(DeltaTime);
    const FVector PrevPos = UpdatedComponent->GetComponentLocation();

    TotalOffset = NewPos - PrevPos;
    UpdatedComponent->AddWorldOffset(TotalOffset, false);
    Velocity = TotalOffset / DeltaTime;
    return;
  }

  // Base forward move
  const FVector ForwardMove = StartVelocity * DeltaTime;

  // Additional motions
  const FVector Offset = ComputeMotionOffset(DeltaTime);

  TotalOffset = ForwardMove + Offset;

  UpdatedComponent->AddWorldOffset(TotalOffset, false);

  Velocity = TotalOffset / DeltaTime;
}

void UAuraProjectileMovementComponent::SetActiveMotion(EProjectileMotionType Motion)
{
  ActiveMotion = Motion;
}

float UAuraProjectileMovementComponent::ComputeEffectiveAmplitude()
{
  float EffectiveAmplitude = Amplitude;
  if (MotionShiftModes.Contains(EMotionShiftMode::Growth))
  {
    EffectiveAmplitude += GrowthRate * ElapsedTime * EffectiveFrequency;
  }

  return EffectiveAmplitude;
}

FVector UAuraProjectileMovementComponent::ComputeYoYoPosition(float DeltaTime)
{
  // Save the current position
  const FVector CurrentPos = UpdatedComponent->GetComponentLocation();

  // Distances / speeds
  const float ForwardSpeed = StartVelocity.Size(); // projectile’s own speed
  const float ForwardDistance = ForwardSpeed * YoYoForwardDuration;
  const float ReturnSpeed = ForwardDistance / YoYoReturnDuration;

  const FVector LocalForward = StartTransform.TransformVectorNoScale(FVector::ForwardVector);

  FVector Step;

  YoYoTravelTime += DeltaTime;

  if (!bYoYoReturning)
  {
    // ---- Forward phase ----
    if (bYoYoInstantTurn)
    {
      Step = LocalForward * ForwardSpeed * DeltaTime;
    }
    else
    {
      YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 0.f, DeltaTime, YoYoInterpSpeed);
      Step = LocalForward * ForwardSpeed * DeltaTime * YoYoSpeedFactor;
    }

    if (YoYoTravelTime >= YoYoForwardDuration)
    {
      bYoYoReturning = true;
      YoYoTravelTime = 0.f;
      YoYoSpeedFactor = bYoYoInstantTurn ? 1.f : 0.f;
    }
  }
  else
  {
    // ---- Return phase ----
    if (bYoYoReturnToAvatar && AvatarActor.IsValid())
    {
      // Return directly to avatar's current location
      const FVector ToTarget = (AvatarActor->GetActorLocation() - CurrentPos).GetSafeNormal();

      if (bYoYoInstantTurn)
      {
        Step = ToTarget * ReturnSpeed * DeltaTime;
      }
      else
      {
        YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 1.f, DeltaTime, YoYoInterpSpeed);
        Step = ToTarget * ReturnSpeed * DeltaTime * YoYoSpeedFactor;
      }
    }
    else
    {
      // Return along string (straight back along launch direction)
      if (bYoYoInstantTurn)
      {
        Step = -LocalForward * ReturnSpeed * DeltaTime;
      }
      else
      {
        YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 1.f, DeltaTime, YoYoInterpSpeed);
        Step = -LocalForward * ReturnSpeed * DeltaTime * YoYoSpeedFactor;
      }
    }
  }

  return CurrentPos + Step;
}

FVector UAuraProjectileMovementComponent::ComputeMotionOffset(float DeltaTime)
{
  FVector Offset = FVector::ZeroVector;

  switch (ActiveMotion)
  {
  case EProjectileMotionType::Helix:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();
      // Build a perpendicular basis around the forward direction
      const FVector Forward = StartVelocity.GetSafeNormal();
      FVector Right, Up;
      Forward.FindBestAxisVectors(Right, Up);

      // Compute circular motion in that plane
      const float Angle = ElapsedTime * EffectiveFrequency + RandomPhaseShift;
      const FVector CircleOffset =
        (FMath::Sin(Angle) * Right + FMath::Cos(Angle) * Up) *
        EffectiveAmplitude * DirectionMultiplier;

      // Add to offset (scaled by DeltaTime to match your system)
      Offset += CircleOffset * DeltaTime;

      break;
    }
  case EProjectileMotionType::Snake:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();

      Offset += FVector(
        0.f,
        FMath::Sin(ElapsedTime * EffectiveFrequency + RandomPhaseShift - PI / 2) * EffectiveAmplitude *
        DirectionMultiplier,
        0.f
      ) * DeltaTime;
      break;
    }

  case EProjectileMotionType::Noise:
    {
      NoiseTimer += DeltaTime;

      const float EffectiveAmplitude = ComputeEffectiveAmplitude();

      if (NoiseTimer >= EffectiveNoiseInterval)
      {
        // Time to pick a new target offset
        TargetNoiseOffset = FVector(
          FMath::FRandRange(-1.f, 1.f) * EffectiveAmplitude,
          FMath::FRandRange(-1.f, 1.f) * EffectiveAmplitude,
          0.f
        );
        NoiseTimer = 0.f;
      }

      CurrentNoiseOffset = FMath::VInterpTo(
        CurrentNoiseOffset,
        TargetNoiseOffset,
        DeltaTime,
        NoiseInterpSpeed
      );

      Offset += CurrentNoiseOffset * DeltaTime;
      break;
    }

  case EProjectileMotionType::Pulse:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();

      Offset += FVector(
        FMath::Cos(ElapsedTime * EffectiveFrequency + RandomPhaseShift) * EffectiveAmplitude * DirectionMultiplier,
        0.f,
        0.f
      ) * DeltaTime;
      break;
    }

  case EProjectileMotionType::ZigZag:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();

      const float Phase = FMath::Sin(ElapsedTime * EffectiveFrequency + RandomPhaseShift - PI / 2);
      Offset += FVector(
        0.f,
        FMath::Sign(Phase) * EffectiveAmplitude * DirectionMultiplier,
        0.f
      ) * DeltaTime;
      break;
    }
  case EProjectileMotionType::YoYo:
    {
      FVector OutOffset;

      YoYoTravelTime += DeltaTime;

      const float ForwardSpeed = StartVelocity.Size(); // actually move forward at projectile’s speed
      const float ForwardDistance = ForwardSpeed * YoYoForwardDuration;
      const float ReturnSpeed = ForwardDistance / YoYoReturnDuration;

      const FVector LocalForward = FVector::ForwardVector;

      if (!bYoYoReturning)
      {
        // ---- Forward phase ----
        YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 1.f, DeltaTime, YoYoInterpSpeed);
        OutOffset = LocalForward * ForwardSpeed * DeltaTime * YoYoSpeedFactor;

        if (YoYoTravelTime >= YoYoForwardDuration)
        {
          bYoYoReturning = true;
          YoYoTravelTime = 0.f;
          YoYoSpeedFactor = 0.f; // reset so return can interp in too
        }
      }
      else
      {
        // ---- Return phase ----
        if (bYoYoReturnToAvatar && AvatarActor.IsValid())
        {
          const FVector ToTarget = (AvatarActor->GetActorLocation() - UpdatedComponent->GetComponentLocation()).
            GetSafeNormal();
          YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 1.f, DeltaTime, YoYoInterpSpeed);
          OutOffset = ToTarget * ReturnSpeed * DeltaTime * YoYoSpeedFactor;
          return OutOffset;
        }

        YoYoSpeedFactor = FMath::FInterpTo(YoYoSpeedFactor, 1.f, DeltaTime, YoYoInterpSpeed);
        OutOffset = -LocalForward * ReturnSpeed * DeltaTime * YoYoSpeedFactor;
      }

      Offset += OutOffset;
      break;
    }
  default:
    {
      break;
    }
  }

  return StartTransform.TransformVectorNoScale(Offset);
}

FVector UAuraProjectileMovementComponent::ComputeBezierPosition(float T) const
{
  const FVector P0 = StartLocation;
  FVector P1 = StartLocation + StartTransform.TransformVectorNoScale(BezierP1);
  FVector P2 = StartLocation + StartTransform.TransformVectorNoScale(BezierP2);
  FVector P3 = StartLocation + StartTransform.TransformVectorNoScale(BezierEnd);

  if (MotionShiftModes.Contains(EMotionShiftMode::ControlPoint))
  {
    P1 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(BezierP1)
    );
    P2 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(BezierP2)
    );
    P3 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(BezierEnd)
    );
  }

  const float U = 1 - T;
  const float TT = T * T;
  const float UU = U * U;
  const float UUU = UU * U;
  const float TTT = TT * T;

  return UUU * P0 +
    3 * UU * T * P1 +
    3 * U * TT * P2 +
    TTT * P3;
}
