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
    case EProjectileMotionType::YoYo:
      {
        Velocity *= RandomSpeedScale;
        StartVelocity = Velocity;
        EffectiveYoYoForwardDuration = YoYoForwardDuration / RandomSpeedScale;
        EffectiveYoYoIdleDuration = YoYoIdleDuration / RandomSpeedScale;
        break;
      }
    default:
      {
        EffectiveFrequency = Frequency * RandomSpeedScale;
      }
    }
  }

  if (MotionShiftModes.Contains(EMotionShiftMode::ControlPoint))
  {
    const float Angle = FMath::FRandRange(-MaxRandomBezierRotation / 2, MaxRandomBezierRotation / 2);
    RandomBezierRotation = FRotator(0.f, Angle, 0.f);
  }

  if (MotionShiftModes.Contains(EMotionShiftMode::YoYoReturnSpeed))
  {
    EffectiveYoYoReturnSpeedFactor = FMath::FRandRange(1.f - MaxSpeedShiftAmplitude, 1.f + MaxSpeedShiftAmplitude);
  }

  if (ActiveMotion == EProjectileMotionType::YoYo)
  {
    SetupYoYoMotion();
  }

  if (bBezierOverride)
  {
    EffectiveBezierEnd = BezierEndOverride;
  }
}

void UAuraProjectileMovementComponent::InitializeComponent()
{
  Super::InitializeComponent();

  EffectiveFrequency = Frequency;
  EffectiveNoiseInterval = NoiseInterval;
  EffectiveBezierDuration = BezierDuration;
  EffectiveBezierEnd = BezierEnd;
  EffectiveYoYoForwardDuration = YoYoForwardDuration;
  EffectiveYoYoIdleDuration = YoYoIdleDuration;
  EffectiveYoYoReturnSpeedFactor = YoYoReturnSpeedFactor;
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
    ActiveMotion == EProjectileMotionType::YoYo ||
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

void UAuraProjectileMovementComponent::SetupYoYoMotion()
{
  // ---- Forward phase ----
  FTimerHandle ForwardPhaseTimer;
  GetOwner()->GetWorldTimerManager().SetTimer(
    ForwardPhaseTimer,
    FTimerDelegate::CreateLambda([this]()
    {
      if (EffectiveYoYoIdleDuration > 0)
      {
        // ---- Idle phase ----
        Velocity = FVector::ZeroVector;
        FTimerHandle IdlePhaseTimer;
        GetOwner()->GetWorldTimerManager().SetTimer(
          IdlePhaseTimer,
          FTimerDelegate::CreateLambda([this]()
          {
            // ---- Return phase ----
            ReturnYoYo();
          }),
          EffectiveYoYoIdleDuration,
          false
        );

        return;
      }

      // ---- Return phase ----
      ReturnYoYo();
    }),
    EffectiveYoYoForwardDuration,
    false
  );
}

FVector UAuraProjectileMovementComponent::ComputeMotionOffset(float DeltaTime)
{
  FVector Offset = FVector::ZeroVector;

  switch (ActiveMotion)
  {
  case EProjectileMotionType::Helix:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();
      const FVector Forward = StartVelocity.GetSafeNormal();
      FVector Right, Up;
      Forward.FindBestAxisVectors(Right, Up);

      const float Angle = ElapsedTime * EffectiveFrequency + RandomPhaseShift;
      const FVector CircleOffset =
        (FMath::Sin(Angle) * Right + FMath::Cos(Angle) * Up) *
        EffectiveAmplitude * DirectionMultiplier;

      Offset += CircleOffset;

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
      );
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

      Offset += CurrentNoiseOffset;
      break;
    }

  case EProjectileMotionType::Pulse:
    {
      const float EffectiveAmplitude = ComputeEffectiveAmplitude();

      Offset += FVector(
        FMath::Cos(ElapsedTime * EffectiveFrequency + RandomPhaseShift) * EffectiveAmplitude * DirectionMultiplier,
        0.f,
        0.f
      );
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
      );
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
  FVector P3 = StartLocation + StartTransform.TransformVectorNoScale(EffectiveBezierEnd);

  if (MotionShiftModes.Contains(EMotionShiftMode::ControlPoint))
  {
    P1 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(BezierP1)
    );
    P2 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(BezierP2)
    );
    P3 = StartLocation + StartTransform.TransformVectorNoScale(
      RandomBezierRotation.RotateVector(EffectiveBezierEnd)
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

void UAuraProjectileMovementComponent::ReturnYoYo()
{
  Velocity = -StartVelocity * EffectiveYoYoReturnSpeedFactor;
  bYoYoReturning = true;

  if (bYoYoReturnToAvatar && AvatarActor.IsValid())
  {
    bIsHomingProjectile = true;
    HomingTargetComponent = AvatarActor->GetRootComponent();
  }
}
