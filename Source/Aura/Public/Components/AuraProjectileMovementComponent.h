// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/MotionShiftMode.h"
#include "Enums/ProjectileMotionType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AuraProjectileMovementComponent.generated.h"

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
  
  void SetBezierEnd(const FVector& InEnd) { BezierEnd = InEnd; }
  
  bool bDefinedDirections = false;
  float DirectionMultiplier = 1.f;

  bool bYoYoReturnToAvatar = false;
  TWeakObjectPtr<AActor> AvatarActor;

protected:
  virtual void BeginPlay() override;
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
  float Frequency = 10.f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Bezier && ActiveMotion != EProjectileMotionType::Default && ActiveMotion != EProjectileMotionType::YoYo", EditConditionHides)
  )
  float Amplitude = 150.f;

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

  /* The max possible speed shift based on a % of Frequency or Bezier Duration */
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
    meta=(EditCondition="ActiveMotion != EProjectileMotionType::Bezier && ActiveMotion != EProjectileMotionType::Default", EditConditionHides)
  )
  float GrowthRate = 150.f;

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
  bool bYoYoInstantTurn = false;
  
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoForwardDuration = 1.0f;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoReturnDuration = 1.0f;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Custom Movement",
    meta=(EditCondition="ActiveMotion == EProjectileMotionType::YoYo", EditConditionHides)
  )
  float YoYoInterpSpeed = 20.0f;


private:
  float ComputeEffectiveAmplitude();
  FVector ComputeYoYoPosition(float DeltaTime);
  FVector ComputeMotionOffset(float DeltaTime);
  FVector ComputeBezierPosition(float T) const;
  
  float ElapsedTime = 0.f;
  float EffectiveFrequency = 10.f;
  
  FVector StartLocation = FVector();
  FVector StartVelocity = FVector();
  FTransform StartTransform = FTransform();
  
  FVector HelixLastLateral = FVector();
  
  float RandomPhaseShift = 0.f;
  
  FRotator RandomBezierRotation = FRotator();
  float EffectiveBezierDuration = 1.f;
  
  float NoiseTimer = 0.f;
  FVector CurrentNoiseOffset = FVector();
  FVector TargetNoiseOffset = FVector();
  float EffectiveNoiseInterval = 0.1f;
  
  bool bYoYoReturning = false;
  float YoYoTravelTime = 0.f;
  float YoYoSpeedFactor = 1.f;
  float YoYoDelayTimer = 0.f;
  
};
