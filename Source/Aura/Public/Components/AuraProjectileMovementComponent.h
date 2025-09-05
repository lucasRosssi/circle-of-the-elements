// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/MotionShiftMode.h"
#include "Enums/ProjectileMotionType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AuraProjectileMovementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnYoYoReturn);

/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
  UAuraProjectileMovementComponent();

  UFUNCTION(BlueprintCallable, Category="Custom Movement")
  void SetActiveMotion(EProjectileMotionType Motion);
  
  bool bDefinedDirections = false;
  float DirectionMultiplier = 1.f;

  bool bYoYoReturnToAvatar = false;
  TWeakObjectPtr<AActor> AvatarActor;

  bool bBezierOverride = false;
  FVector BezierEndOverride = FVector();

  FOnYoYoReturn OnYoYoReturnDelegate;

protected:
  virtual void BeginPlay() override;
  virtual void InitializeComponent() override;
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  /** The currently active motion effect */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Custom Movement")
  EProjectileMotionType ActiveMotion = EProjectileMotionType::Default;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Bezier && ActiveMotion != EProjectileMotionType::Default && ActiveMotion != EProjectileMotionType::Noise && ActiveMotion != EProjectileMotionType::YoYo", EditConditionHides)
  )
  float Frequency = 15.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Bezier && ActiveMotion != EProjectileMotionType::Default && ActiveMotion != EProjectileMotionType::YoYo", EditConditionHides)
  )
  float Amplitude = 8.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Noise", EditConditionHides)
  )
  float NoiseInterval = 0.1f;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Noise", EditConditionHides)
  )
  float NoiseInterpSpeed = 25.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement|Motion Shift",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Default", EditConditionHides)
  )
  TSet<EMotionShiftMode> MotionShiftModes;

  /* The max possible speed shift based on a % of Frequency, Bezier Duration or Velocity */
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement|Motion Shift",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Default", EditConditionHides)
  )
  float MaxSpeedShiftAmplitude = 1.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement|Motion Shift",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Bezier && ActiveMotion != EProjectileMotionType::Default && ActiveMotion != EProjectileMotionType::YoYo", EditConditionHides)
  )
  float GrowthRate = 1.f;

  /** Bezier control points */
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Bezier", EditConditionHides)
  )
  FVector BezierP1 = FVector();

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Bezier", EditConditionHides)
  )
  FVector BezierP2 = FVector();

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Bezier", EditConditionHides)
  )
  FVector BezierEnd = FVector();

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::Bezier", EditConditionHides)
  )
  float BezierDuration = 1.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement|Motion Shift",
    meta=(
      EditCondition="ActiveMotion == EProjectileMotionType::Bezier",
      EditConditionHides,
      ClampMin=-120.f,
      UIMin=-120.f,
      ClampMax=120.f,
      UIMax=120.f
    )
  )
  float MaxRandomBezierRotation = 60.f;
  
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoForwardDuration = 0.5f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoIdleDuration = 0.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoReturnSpeedFactor = 1.f;

private:
  float ComputeEffectiveAmplitude();
  FVector ComputeMotionOffset(float DeltaTime);
  FVector ComputeBezierPosition(float T) const;

  void SetupYoYoMotion();
  void ReturnYoYo();
  
  float ElapsedTime = 0.f;
  float EffectiveFrequency = 10.f;
  
  
  FVector StartLocation = FVector();
  FVector StartVelocity = FVector();
  FTransform StartTransform = FTransform();
  
  FVector HelixLastLateral = FVector();
  
  float RandomPhaseShift = 0.f;
  
  FRotator RandomBezierRotation = FRotator();
  float EffectiveBezierDuration = 1.f;
  FVector EffectiveBezierEnd = FVector();
  
  float NoiseTimer = 0.f;
  FVector CurrentNoiseOffset = FVector();
  FVector TargetNoiseOffset = FVector();
  float EffectiveNoiseInterval = 0.1f;

  bool bYoYoReturning = false;
  float EffectiveYoYoForwardDuration = 0.5f;
  float EffectiveYoYoIdleDuration = 0.f;
  float EffectiveYoYoReturnSpeedFactor = 1.f;
};
