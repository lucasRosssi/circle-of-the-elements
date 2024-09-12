// Universal Camera Plugin - Mathieu Jacq 2021


#include "UniversalCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AsyncNode.h"
#include "Curves/CurveFloat.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Logging/MessageLog.h" 
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "UniversalCameraSaveGame.h"
#include "Math/Box.h"
#include "Engine/Engine.h"
#include "Components/SplineComponent.h"

FVector FTargetSettings_WithTemplates::GetTargetLocation(bool& IsValid, FRotator& Rotation)
{
	IsValid = false;

	switch (Type)
	{
	case 0:
		if (TargetSettings.IsValidActor())
		{
			IsValid = true;
			Rotation = TargetSettings.GetActorRotation();
			return TargetSettings.GetActorLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	case 1:
		if (TargetSettings.IsValidSceneComponent())
		{
			IsValid = true;
			Rotation = TargetSettings.GetSceneComponentRotation();
			return TargetSettings.GetSceneComponentLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	case 2:
		if (TargetSettings.IsValidSocket())
		{
			IsValid = true;
			Rotation = TargetSettings.GetSocketRotation();
			return TargetSettings.GetSocketLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	}

	return FVector(0.f, 0.f, 0.f);
}


// Sets default values
AUniversalCamera::AUniversalCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
}


void AUniversalCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUniversalCamera, DesiredLocation);
	DOREPLIFETIME(AUniversalCamera, DesiredSocketOffset);
	DOREPLIFETIME(AUniversalCamera, DesiredTargetOffset);
	DOREPLIFETIME(AUniversalCamera, DesiredRotation);
	DOREPLIFETIME(AUniversalCamera, DesiredZoom);
	DOREPLIFETIME(AUniversalCamera, DesiredRotationOffset);
}

// Called when the game starts or when spawned
void AUniversalCamera::BeginPlay()
{
	LOG_VerifyStartingSettings();

	if (GetWorld()) LastDeltaTime = GetWorld()->GetDeltaSeconds();
	
	SetDesiredLocation(OverrideStartingLocation ? StartingLocation : GetActorLocation(), true, false);
	SetDesiredSocketOffset(OverrideStartingSocketOffset ? StartingSocketOffset : SpringArmComponent->SocketOffset, true, false);
	SetDesiredTargetOffset(OverrideStartingTargetOffset ? StartingTargetOffset : SpringArmComponent->TargetOffset, true, false);
	SetDesiredYaw(OverrideStartingYaw ? StartingYaw : GetActorRotation().Yaw, true, false);
	SetDesiredPitch(OverrideStartingPitch ? StartingPitch : GetActorRotation().Pitch, true, false);
	SetDesiredZoom(OverrideStartingZoom ? StartingZoom : SpringArmComponent->TargetArmLength, true, false);

	Super::BeginPlay();
}

void AUniversalCamera::LOG_VerifyStartingSettings()
{
	//	Verify that a CameraComponent is attached to the SpringArmComponent
	bool HasCameraAttached;
	GetCameraComponent(HasCameraAttached);
	if (!HasCameraAttached)
	{
		FMessageLog PIELogger = FMessageLog(FName("PIE"));
		PIELogger.Warning(FText::FromString("Please attach a Camera or a CineCamera to the SpringArmComponent of your Universal Camera class!"));
	}

	//	Verify that DoCollisionTest is disabled
	if (SpringArmComponent->bDoCollisionTest)
	{
		FMessageLog PIELogger = FMessageLog(FName("PIE"));
		PIELogger.Warning(FText::FromString("Please disable 'DoCollisionTest' in the SpringArmComponent of your Universal Camera and use the 'SpringArmCollisions' feature in your Universal Camera defaults instead."));
	}

	//	Verify that OriginArmCollisions probe size is greater that SpringArmCollisions probe size
	if (EnabledFeatures & OriginCollisions && EnabledFeatures & SpringArmCollisions && (SpringArmCollisionsProbeSize > OriginCollisionsProbeSize))
	{
		FMessageLog PIELogger = FMessageLog(FName("PIE"));
		PIELogger.Warning(FText::FromString("Spring Arm Collisions Probe Size is greater than Origin Collisions Probe Size, which could result in Spring Arm Collisions triggering when the Origin is near geometry."));
	}
}

// Called every frame
void AUniversalCamera::Tick(float DeltaTime)
{
	const float DeltaTimeCorrective = DeltaTime * GetTimeDilationCorrectiveValue();

	LastLocation = GetActorLocation();

	if (EnabledFeatures & EdgeScrolling) EdgeScrollingTick_Internal();
		
	if (IsScreenSliding) ScreenSlidingTick();

	if (IsTraveling) TravelTaskTick(DeltaTimeCorrective);

	if (bIsFollowingAnyActor) FollowTargetTick();

	//	Update Z Desired Location if using Terrain Height Adaptation
	if ((EnabledFeatures & TerrainHeightAdaptation) && !IsPreciseMouseDragging && !IsFollowingAnyActor())
	{
		ApplyTerrainHeightAdaptationTick();
	}

	if (IsPreciseMouseDragging)
	{
		PreciseMouseDragTick();
	}

	//	LAG
	
	if (EnabledFeatures & LagMovement)
	{
		SetActorLocation(Lag_GetTickDesiredLocation(DeltaTimeCorrective));
	}
	if (EnabledFeatures & LagOffset)
	{
		SpringArmComponent->SocketOffset = Lag_GetTickDesiredSocketOffset(DeltaTimeCorrective);
		SpringArmComponent->TargetOffset = Lag_GetTickDesiredTargetOffset(DeltaTimeCorrective);
	}
	if (EnabledFeatures & LagMovement)
	{
		FVector CurrentLocation = GetActorLocation();
		SetActorLocation(FVector(CurrentLocation.X, CurrentLocation.Y, Lag_GetTickDesiredHeight(DeltaTimeCorrective)));
	}
	if (EnabledFeatures & LagRotation)
	{
		SetActorRotation(Lag_GetTickDesiredRotation(DeltaTimeCorrective));
	}
	
	UpdateSpringArmLength(DeltaTimeCorrective);
	
	if (DebugPrintDesiredPosition) PrintDesiredPosition();

	//	Update Velocity
	Velocity = (GetActorLocation() - LastLocation) / LastDeltaTime;
	if (GetWorld()) LastDeltaTime = GetWorld()->GetDeltaSeconds();

	LastTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	Super::Tick(DeltaTime);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (DrawDebugSpheres)
	{
		FVector CurrentLocation = GetActorLocation();
		DrawDebugSphere(GetWorld(), GetActorLocation(), 5.f, 8, FColor::Green);

		if (EnabledFeatures & LagMovement)
		{
			DrawDebugSphere(GetWorld(), DesiredLocation, 5.f, 8, FColor::Yellow);
			DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, DesiredLocation, 7.5f, FColor::Yellow);
		}
		if (DesiredTargetOffset.Length() > KINDA_SMALL_NUMBER)
		{
			DrawDebugSphere(GetWorld(), GetDesiredTargetOffsetWorldLocation(), 5.f, 8, FColor::Orange);
			DrawDebugDirectionalArrow(GetWorld(), DesiredLocation, GetDesiredTargetOffsetWorldLocation(), 7.5f, FColor::Orange);
		}
		if (DesiredSocketOffset.Length() > KINDA_SMALL_NUMBER)
		{
			DrawDebugSphere(GetWorld(), GetDesiredSocketOffsetWorldLocation(), 5.f, 8, FColor::Red);
			DrawDebugDirectionalArrow(GetWorld(), GetDesiredTargetOffsetWorldLocation(), GetDesiredSocketOffsetWorldLocation(), 7.5f, FColor::Red);
		}
	}
#endif
}

// Called to bind functionality to input
void AUniversalCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UCameraComponent* AUniversalCamera::GetCameraComponent(bool& HasCameraAttached)
{
	TArray<TObjectPtr<USceneComponent>> AttachedChildren = SpringArmComponent->GetAttachChildren();
	HasCameraAttached = false;

	for (TObjectPtr<USceneComponent> AttachedChild : AttachedChildren)
	{
		UCameraComponent* Camera = Cast<UCameraComponent>(AttachedChild);
		if (IsValid(Camera))
		{
			HasCameraAttached = true;
			return Camera;
		}
	}

	return nullptr;
}

void AUniversalCamera::UpdateSpringArmLength(float DeltaTime)
{
	float NewTargetArmLength = SpringArmComponent->TargetArmLength;

	if (EnabledFeatures & LagZoom)
	{
		NewTargetArmLength = Lag_GetTickDesiredZoom(DeltaTime);
	}

	if (EnabledFeatures & SpringArmCollisions && (NewTargetArmLength != 0.0f))
	{
		FVector OriginLoc = GetActorLocation();
		FVector CameraLoc = OriginLoc - GetActorRotation().Vector() * NewTargetArmLength;

		FHitResult HitResult;

		AActor* ActorToIgnore = IsFollowingAnyActor() ? FollowTargetSettings.TargetSettings.GetOwnerActor() : GetOwner();
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, ActorToIgnore);
		GetWorld()->SweepSingleByChannel(HitResult, OriginLoc, CameraLoc, FQuat::Identity, SpringArmCollisionsChannel, FCollisionShape::MakeSphere(SpringArmCollisionsProbeSize), QueryParams);

		if (HitResult.bBlockingHit)
		{
			NewTargetArmLength = (HitResult.Location - OriginLoc).Length();
		}
	}

	SpringArmComponent->TargetArmLength = NewTargetArmLength;
}


void AUniversalCamera::EnableFeatures(int32 FeaturesToEnable)
{
	EnabledFeatures |= FeaturesToEnable;
}

void AUniversalCamera::DisableFeatures(int32 FeaturesToDisable)
{
	//	Disable ScreenSliding if active
	if ((FeaturesToDisable & ScreenSliding) && (EnabledFeatures & ScreenSliding) && IsScreenSliding)
	{
		ToggleScreenSliding(false);
	}
	//	Stop Following if active
	if ((FeaturesToDisable & EEnabledFeatures::FollowTarget) && (EnabledFeatures & EEnabledFeatures::FollowTarget) && IsFollowingAnyActor())
	{
		StopFollowing_Internal(bShouldResetOffsetIfFailed, StopFollowingReason_DisabledFeature);
	}
	//	Disable PreciseMouseDrag if active
	if ((FeaturesToDisable & PreciseMouseDrag) && (EnabledFeatures & PreciseMouseDrag) && IsPreciseMouseDragging)
	{
		TogglePreciseMouseDrag(false);
	}

	if ((FeaturesToDisable & LagMovement) && (EnabledFeatures & LagMovement))
	{
		DesiredLocation = GetActorLocation();
	}
	if ((FeaturesToDisable & LagOffset) && (EnabledFeatures & LagOffset))
	{
		DesiredSocketOffset = SpringArmComponent->SocketOffset;
		DesiredTargetOffset = SpringArmComponent->TargetOffset;
	}
	if ((FeaturesToDisable & LagRotation) && (EnabledFeatures & LagRotation))
	{
		DesiredRotation = GetActorRotation();
	}
	if ((FeaturesToDisable & LagZoom) && (EnabledFeatures & LagZoom))
	{
		DesiredZoom = SpringArmComponent->TargetArmLength;
	}

	EnabledFeatures = ~(~EnabledFeatures | FeaturesToDisable);
}

bool AUniversalCamera::AreFeaturesEnabled(int32 FeaturesToTest)
{
	return (EnabledFeatures & FeaturesToTest);
}

void AUniversalCamera::StartTraveling_Internal(UAsyncNode* AsyncNode)
{
	if (!IsValid(AsyncNode)) return;

	CurrentTravelTask = AsyncNode;
	IsTraveling = true;
}

void AUniversalCamera::TravelTaskTick(float DeltaTime)
{ 
	if (!IsValid(CurrentTravelTask))
	{
		EndTravelTask(true);
		return;
	}
	
	CurrentTravelTask->TryUpdateLocationDirectionAndValidity();
	CurrentTravelTask->TryUpdateYawDirectionAndValidity();
	CurrentTravelTask->TryUpdatePitchDirectionAndValidity();
	CurrentTravelTask->TryUpdateRollDirectionAndValidity();
	CurrentTravelTask->TryUpdateZoomDirectionAndValidity();

	if (!CurrentTravelTask->AreValidSettings())
	{
		EndTravelTask(true);
		return;
	}

	CurrentTravelTask->ElapsedTime += DeltaTime;

	float CurvePosition = CurrentTravelTask->ElapsedTime / CurrentTravelTask->MaxDuration;
	float CurveValue = (CurrentTravelTask->m_Curve != nullptr) ? CurrentTravelTask->m_Curve->GetFloatValue(CurvePosition) : CurvePosition;

	UpdateCameraTransform(CurveValue);

	if (CurvePosition >= 1.f)
	{
		EndTravelTask(false);
	}
}

void AUniversalCamera::UpdateCameraTransform(float CurveValue)
{
	const bool UseSpline = CurrentTravelTask->LocationSettings.TargetMod == ETargetMod::TargetMod_Spline;

	if (UseSpline)
	{
		USplineComponent* SplineComponent = CurrentTravelTask->LocationSettings.SplineComponent;
		if (SplineComponent)
		{
			const float SplineLength = SplineComponent->GetSplineLength();
			SetDesiredLocation_Internal(SplineComponent->GetLocationAtDistanceAlongSpline(CurveValue * SplineLength, ESplineCoordinateSpace::World));
		}
	}
	else
	{
		if (CurrentTravelTask->IsValidLocation)
		{
			SetDesiredLocation_Internal(CurrentTravelTask->StartingLocation + CurrentTravelTask->LocationDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
		}
	}

	if (CurrentTravelTask->IsValidOffset)
	{
		if (CurrentTravelTask->OffsetSettings.OffsetMod == 1)
		{
			SetDesiredTargetOffset_Internal(CurrentTravelTask->StartingOffset + CurrentTravelTask->OffsetDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
		}
		else
		{
			SetDesiredSocketOffset_Internal(CurrentTravelTask->StartingOffset + CurrentTravelTask->OffsetDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
		}
	}
	if (CurrentTravelTask->IsValidYaw)
	{
		SetDesiredYaw_Internal(CurrentTravelTask->StartingYaw + CurrentTravelTask->YawDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
	}
	if (CurrentTravelTask->IsValidPitch)
	{
		SetDesiredPitch_Internal(CurrentTravelTask->StartingPitch + CurrentTravelTask->PitchDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
	}
	if (CurrentTravelTask->IsValidRoll)
	{
		SetDesiredRoll_Internal(CurrentTravelTask->StartingRoll + CurrentTravelTask->RollDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
	}
	if (CurrentTravelTask->IsValidZoom)
	{
		SetDesiredZoom_Internal(CurrentTravelTask->StartingZoom + CurrentTravelTask->ZoomDirection * CurveValue, CurrentTravelTask->m_IgnoreLag, CurrentTravelTask->m_IgnoreRestrictions);
	}
}

void AUniversalCamera::EndTravelTask(const bool Abort)
{
	IsTraveling = false;

	if (Abort && IsValid(CurrentTravelTask))
	{
		CurrentTravelTask->OnAborted.Broadcast();
	}
	else if (IsValid(CurrentTravelTask))
	{
		CurrentTravelTask->OnReachedDestination.Broadcast();
	}
}

void AUniversalCamera::SetDesiredPosition(FVector NewLocation, FVector NewSocketOffset, FVector NewTargetOffset, float NewYaw, float NewPitch, float NewRoll, float NewZoom, bool IgnoreLag, bool IgnoreRestrictions)
{
	SetDesiredLocation(NewLocation, IgnoreLag, IgnoreRestrictions);
	SetDesiredSocketOffset(NewSocketOffset, IgnoreLag, IgnoreRestrictions);
	SetDesiredTargetOffset(NewTargetOffset, IgnoreLag, IgnoreRestrictions);
	SetDesiredYaw(NewYaw, IgnoreLag, IgnoreRestrictions);
	SetDesiredPitch(NewPitch, IgnoreLag, IgnoreRestrictions);
	SetDesiredRoll(NewRoll, IgnoreLag, IgnoreRestrictions);
	SetDesiredZoom(NewZoom, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredLocation_Internal(FVector NewLocation, bool IgnoreLag, bool IgnoreRestrictions)
{ 
	if (!IgnoreRestrictions) NewLocation = GetCorrectedDestinationFromRestrictions(NewLocation);

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions)
	{
		bool DumBool;
		NewLocation = GetCorrectedDestinationFromOriginCollisions(DesiredLocation, NewLocation, DumBool);
	}

	DesiredLocation = NewLocation; 
	if (!(EnabledFeatures & EEnabledFeatures::LagMovement) || IgnoreLag)
	{
		LastLocation = GetActorLocation();
		LastDeltaTime = GetWorld()->GetDeltaSeconds();
		SetActorLocation(DesiredLocation, true);
	}
		
}

void AUniversalCamera::SetDesiredLocation(FVector NewLocation, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || CurrentTravelTask->IsValidLocation)) return;

	SetDesiredLocation_Internal(NewLocation, IgnoreLag, IgnoreRestrictions);
}

FVector AUniversalCamera::GetClampedLocation(FVector Location)
{
	// UE5 compatibility / replaces FVector member references
	float X = Location.X;
	float Y = Location.Y;
	float Z = Location.Z;

	ClampCustomValue(ConstrainLocationMinimum.X, ConstrainLocationMaximum.X, &X, MinimumLocation.X, MaximumLocation.X);
	ClampCustomValue(ConstrainLocationMinimum.Y, ConstrainLocationMaximum.Y, &Y, MinimumLocation.Y, MaximumLocation.Y);
	ClampCustomValue(ConstrainLocationMinimum.Z, ConstrainLocationMaximum.Z, &Z, MinimumLocation.Z, MaximumLocation.Z);

	return FVector(X, Y, Z);
}

void AUniversalCamera::SetDesiredTargetOffset_Internal(FVector NewTargetOffset, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (!IgnoreRestrictions) NewTargetOffset = GetClampedOffset(NewTargetOffset);

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions)
	{
		bool DumBool;
		NewTargetOffset = GetCorrectedDestinationFromOriginCollisions(GetDesiredTargetOffsetWorldLocation(), DesiredLocation + NewTargetOffset, DumBool) - DesiredLocation;
	}

	DesiredTargetOffset = NewTargetOffset;

	if (!(EnabledFeatures & EEnabledFeatures::LagOffset) || IgnoreLag)
		SpringArmComponent->TargetOffset = DesiredTargetOffset;
}

void AUniversalCamera::SetDesiredTargetOffset(FVector NewTargetOffset, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || (CurrentTravelTask->IsValidOffset && CurrentTravelTask->OffsetSettings.OffsetMod == 1))) return;

	SetDesiredTargetOffset_Internal(NewTargetOffset, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredSocketOffset_Internal(FVector NewSocketOffset, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (!IgnoreRestrictions) NewSocketOffset = GetClampedOffset(NewSocketOffset);

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions)
	{
		bool DumBool;
		FVector NewWorldSocketOffset = GetCorrectedDestinationFromOriginCollisions(GetDesiredSocketOffsetWorldLocation(), GetDesiredTargetOffsetWorldLocation() + FRotationMatrix(DesiredRotation).TransformVector(NewSocketOffset), DumBool);

		FVector RelativeLoc = FTransform(DesiredRotation, GetDesiredTargetOffsetWorldLocation()).InverseTransformPosition(NewWorldSocketOffset);
		NewSocketOffset = RelativeLoc;
	}

	DesiredSocketOffset = NewSocketOffset;
	if (!(EnabledFeatures & EEnabledFeatures::LagOffset) || IgnoreLag)
		SpringArmComponent->SocketOffset = DesiredSocketOffset;
}

void AUniversalCamera::SetDesiredSocketOffset(FVector NewSocketOffset, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || (CurrentTravelTask->IsValidOffset && CurrentTravelTask->OffsetSettings.OffsetMod == 2))) return;

	SetDesiredSocketOffset_Internal(NewSocketOffset, IgnoreLag, IgnoreRestrictions);
}

FVector AUniversalCamera::GetClampedOffset(FVector Offset)
{
	if (OffsetConstrainType == EConstrainType::CustomConstrain)
	{
		// UE5.0 compatibility / Replaces FVector member references

		float X = Offset.X;
		float Y = Offset.Y;
		float Z = Offset.Z;

		ClampCustomValue(ConstrainOffsetMinimum.X, ConstrainOffsetMaximum.X, &X, MinimumOffset.X, MaximumOffset.X);
		ClampCustomValue(ConstrainOffsetMinimum.Y, ConstrainOffsetMaximum.Y, &Y, MinimumOffset.Y, MaximumOffset.Y);
		ClampCustomValue(ConstrainOffsetMinimum.Z, ConstrainOffsetMaximum.Z, &Z, MinimumOffset.Z, MaximumOffset.Z);

		Offset = FVector(X, Y, Z);
	}
	else if (OffsetConstrainType == EConstrainType::MaxDistance)
	{
		if (Offset.Length() > OffsetMaxDistance)
		{
			FVector Direction = Offset;
			Direction.Normalize();
			Offset = Direction * OffsetMaxDistance;
		}
	}
	return Offset;
}

void AUniversalCamera::SetDesiredYaw_Internal(float NewYaw, bool IgnoreLag, bool IgnoreRestrictions)
{
	float NewYawOffset = DesiredRotationOffset.Yaw;
	if (!IgnoreRestrictions)
	{
		NewYaw = GetClampedYaw(NewYaw);
		NewYawOffset = GetClampedYaw(NewYaw + DesiredRotationOffset.Yaw) - NewYaw;
	}

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions && DesiredSocketOffset.Length() > KINDA_SMALL_NUMBER)
	{
		bool DidHit;
		FVector NewWorldSocketOffset = GetCorrectedDestinationFromOriginCollisions(GetDesiredSocketOffsetWorldLocation(), GetDesiredTargetOffsetWorldLocation() + FRotationMatrix(FRotator(DesiredRotation.Pitch, NewYaw, DesiredRotation.Roll)).TransformVector(DesiredSocketOffset), DidHit);

		if (DidHit)
		{
			return;
		}
	}

	DesiredRotation.Yaw = NewYaw;
	DesiredRotationOffset.Yaw = NewYawOffset;
	if (!(EnabledFeatures & EEnabledFeatures::LagRotation) || IgnoreLag)
		SetActorRotation(GetDesiredRotationWithOffset());
}

void AUniversalCamera::SetDesiredYaw(float NewYaw, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || CurrentTravelTask->IsValidYaw)) return;

	SetDesiredYaw_Internal(NewYaw, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredYawOffset(float NewYawOffset)
{
	DesiredRotationOffset.Yaw = NewYawOffset;
	SetDesiredYaw(DesiredRotation.Yaw);
}

float AUniversalCamera::GetClampedYaw(float Yaw)
{
	Yaw = UKismetMathLibrary::NormalizeAxis(Yaw);
	ClampCustomValue(ConstrainYaw, ConstrainYaw, &Yaw, MinimumYaw, MaximumYaw, ReverseYawRestriction);
	return Yaw;
}

void AUniversalCamera::SetDesiredPitch_Internal(float NewPitch, bool IgnoreLag, bool IgnoreRestrictions)
{
	float NewPitchOffset = DesiredRotationOffset.Pitch;
	if (!IgnoreRestrictions)
	{
		NewPitch = GetClampedPitch(NewPitch);
		NewPitchOffset = GetClampedPitch(NewPitch + DesiredRotationOffset.Pitch) - NewPitch;
	}

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions && DesiredSocketOffset.Length() > KINDA_SMALL_NUMBER)
	{
		bool DidHit;
		FVector NewWorldSocketOffset = GetCorrectedDestinationFromOriginCollisions(GetDesiredSocketOffsetWorldLocation(), GetDesiredTargetOffsetWorldLocation() + FRotationMatrix(FRotator(NewPitch, DesiredRotation.Yaw, DesiredRotation.Roll)).TransformVector(DesiredSocketOffset), DidHit);

		if (DidHit)
		{
			return;
		}
	}

	DesiredRotation.Pitch = NewPitch;
	DesiredRotationOffset.Pitch = NewPitchOffset;
	if (!(EnabledFeatures & EEnabledFeatures::LagRotation) || IgnoreLag)
		SetActorRotation(GetDesiredRotationWithOffset());
}

void AUniversalCamera::SetDesiredPitch(float NewPitch, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || CurrentTravelTask->IsValidPitch)) return;

	SetDesiredPitch_Internal(NewPitch, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredPitchOffset(float NewPitchOffset)
{
	DesiredRotationOffset.Pitch = NewPitchOffset;
	SetDesiredPitch(DesiredRotation.Pitch);
}

float AUniversalCamera::GetClampedPitch(float Pitch)
{
	Pitch = UKismetMathLibrary::NormalizeAxis(Pitch);
	ClampCustomValue(ConstrainPitch, ConstrainPitch, &Pitch, MinimumPitch, MaximumPitch, ReversePitchRestriction);
	return Pitch;
}

void AUniversalCamera::SetDesiredRoll_Internal(float NewRoll, bool IgnoreLag, bool IgnoreRestrictions)
{
	float NewRollOffset = DesiredRotationOffset.Roll;
	if (!IgnoreRestrictions)
	{
		NewRoll = GetClampedRoll(NewRoll);
		NewRollOffset = GetClampedRoll(NewRoll + DesiredRotationOffset.Roll) - NewRoll;
	}

	if (!IgnoreRestrictions && EnabledFeatures & OriginCollisions && DesiredSocketOffset.Length() > KINDA_SMALL_NUMBER)
	{
		bool DidHit;
		FVector NewWorldSocketOffset = GetCorrectedDestinationFromOriginCollisions(GetDesiredSocketOffsetWorldLocation(), GetDesiredTargetOffsetWorldLocation() + FRotationMatrix(FRotator(DesiredRotation.Pitch, DesiredRotation.Yaw, NewRoll)).TransformVector(DesiredSocketOffset), DidHit);

		if (DidHit)
		{
			return;
		}
	}

	DesiredRotation.Roll = NewRoll;
	DesiredRotationOffset.Roll = NewRollOffset;
	if (!(EnabledFeatures & EEnabledFeatures::LagRotation) || IgnoreLag)
		SetActorRotation(GetDesiredRotationWithOffset());
}

void AUniversalCamera::SetDesiredRoll(float NewRoll, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || CurrentTravelTask->IsValidRoll)) return;

	SetDesiredRoll_Internal(NewRoll, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredRollOffset(float NewRollOffset)
{
	DesiredRotationOffset.Roll = NewRollOffset;
	SetDesiredRoll(DesiredRotation.Roll);
}

float AUniversalCamera::GetClampedRoll(float Roll)
{
	Roll = UKismetMathLibrary::NormalizeAxis(Roll);
	ClampCustomValue(ConstrainRoll, ConstrainRoll, &Roll, MinimumRoll, MaximumRoll, ReverseRollRestriction);
	return Roll;
}

void AUniversalCamera::SetDesiredRotation(FRotator NewRotation, bool IgnoreLag, bool IgnoreRestrictions)
{
	SetDesiredYaw(NewRotation.Yaw, IgnoreLag, IgnoreRestrictions);
	SetDesiredPitch(NewRotation.Pitch, IgnoreLag, IgnoreRestrictions);
	SetDesiredRoll(NewRotation.Roll, IgnoreLag, IgnoreRestrictions);
}

void AUniversalCamera::SetDesiredRotationOffset(FRotator NewRotationOffset)
{
	DesiredRotationOffset = NewRotationOffset;
	SetDesiredRotation(DesiredRotation);
}

void AUniversalCamera::SetDesiredZoom_Internal(float NewZoom, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (!IgnoreRestrictions) NewZoom = GetClampedZoom(NewZoom);

	DesiredZoom = NewZoom;
	if (!(EnabledFeatures & EEnabledFeatures::LagZoom) || IgnoreLag)
		SpringArmComponent->TargetArmLength = DesiredZoom;
}

void AUniversalCamera::SetDesiredZoom(float NewZoom, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (IsTraveling && IsValid(CurrentTravelTask) && (CurrentTravelTask->m_LockAllMovement || CurrentTravelTask->IsValidZoom)) return;

	SetDesiredZoom_Internal(NewZoom, IgnoreLag, IgnoreRestrictions);
}

FVector AUniversalCamera::GetOriginLocationFromCameraLocation(FVector CameraLocation) const
{
	return CameraLocation - DesiredTargetOffset - FRotationMatrix(DesiredRotation).TransformVector(DesiredSocketOffset) + DesiredRotation.Vector() * DesiredZoom;
}

float AUniversalCamera::GetClampedZoom(float Zoom)
{
	ClampCustomValue(ConstrainZoomMinimum, ConstrainZoomMaximum, &Zoom, MinimumZoom, MaximumZoom);
	return Zoom;
}

void AUniversalCamera::EdgeScrollingTick_Internal()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	//	Get Mouse Position
	float MouseX, MouseY;
	if (!PlayerController->GetMousePosition(MouseX, MouseY)) return;

	//	Get Screen Size
	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);

	float RightMultiplier = 0.f;
	float ForwardMultiplier = 0.f;

	float XSensitivity = EdgeScrollingSensitivity * SizeX;
	float YSensitivity = EdgeScrollingSensitivity * SizeY;

	float RightAxis = 0.f;
	float ForwardAxis = 0.f;

	bool bTargetOffset = false;
	bool bSocketOffset = false;

	bool bShouldEdgeScroll = false;

	switch (EdgeScrollMode)
	{
	case ScrollMovement:
		bTargetOffset = false;
		bSocketOffset = false;
		break;
	case ScrollSocketOffset:
		bTargetOffset = true;
		bSocketOffset = false;
		break;
	case ScrollTargetOffset:
		bTargetOffset = false;
		bSocketOffset = true;
		break;
	}

	if (MouseX < XSensitivity)
	{
		RightAxis = -1.f;
		RightMultiplier = UseEdgeScrollingStrength ? 1.0f - MouseX / XSensitivity : 1.0f;
		bShouldEdgeScroll = true;
	}
	else if (MouseX > SizeX - XSensitivity)
	{
		RightAxis = 1.f;
		RightMultiplier = UseEdgeScrollingStrength ? 1.0f - (SizeX - MouseX) / XSensitivity : 1.0f;
		bShouldEdgeScroll = true;
	}
	if (MouseY < YSensitivity)
	{
		ForwardAxis = 1.f;
		ForwardMultiplier = UseEdgeScrollingStrength ? 1.0f - MouseY / YSensitivity : 1.0f;
		bShouldEdgeScroll = true;
	}
	else if (MouseY > SizeY - YSensitivity)
	{
		ForwardAxis = -1.f;
		ForwardMultiplier = UseEdgeScrollingStrength ? 1.0f - (SizeY - MouseY) / YSensitivity : 1.0f;
		bShouldEdgeScroll = true;
	}

	FVector2D DirectionalMultiplier(FMath::Abs(RightAxis), FMath::Abs(ForwardAxis));
	if (NormalizeEdgeScrollingDirection) DirectionalMultiplier.Normalize();
	DirectionalMultiplier.X *= RightMultiplier * EdgeScrollingSpeedMultiplier;
	DirectionalMultiplier.Y *= ForwardMultiplier * EdgeScrollingSpeedMultiplier;

	if (bShouldEdgeScroll)
	{
		EdgeScrollingTick(RightAxis, ForwardAxis, DirectionalMultiplier.X, DirectionalMultiplier.Y, bTargetOffset, bSocketOffset);
	}

	if (!IsEdgeScrolling && bShouldEdgeScroll)
	{
		IsEdgeScrolling = true;
		OnBeginEdgeScrolling();
	}

	if (IsEdgeScrolling && !bShouldEdgeScroll)
	{
		IsEdgeScrolling = false;
		OnEndEdgeScrolling();
	}

	ESimplifiedMovementDirection SimplifiedMovementDirection = MovementDirection_None;

	if (IsEdgeScrolling)
	{
		if (RightAxis > 0.f)
		{
			if (ForwardAxis > 0.f)
			{
				SimplifiedMovementDirection = Direction_RightForward;
			}
			else if (ForwardAxis < 0.f)
			{
				SimplifiedMovementDirection = Direction_RightBackward;
			}
			else
			{
				SimplifiedMovementDirection = Direction_Right;
			}
		}
		else if (RightAxis < 0.f)
		{
			if (ForwardAxis > 0.f)
			{
				SimplifiedMovementDirection = Direction_LeftForward;
			}
			else if (ForwardAxis < 0.f)
			{
				SimplifiedMovementDirection = Direction_LeftBackward;
			}
			else
			{
				SimplifiedMovementDirection = Direction_Left;
			}
		}
		else
		{
			if (ForwardAxis > 0.f)
			{
				SimplifiedMovementDirection = Direction_Forward;
			}
			else if (ForwardAxis < 0.f)
			{
				SimplifiedMovementDirection = Direction_Backward;
			}
		}
		FVector2D Direction = FVector2D(ForwardAxis * ForwardMultiplier, RightAxis * RightMultiplier);
		OnUpdateEdgeScrolling(SimplifiedMovementDirection, Direction);
	}
}

void AUniversalCamera::EdgeScrollingTick_Implementation(float RightAxis, float ForwardAxis, float RightMultiplier, float ForwardMultiplier, bool bTargetOffset, bool bSocketOffset)
{
	MoveRight(RightAxis, RightMultiplier, bTargetOffset, bSocketOffset);
	MoveForward(ForwardAxis, ForwardMultiplier, bTargetOffset, bSocketOffset);
}

FVector AUniversalCamera::Lag_GetTickDesiredLocation_Implementation(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector2D TickDesiredLocation = FMath::Vector2DInterpTo(FVector2D(CurrentLocation.X, CurrentLocation.Y), FVector2D(DesiredLocation.X, DesiredLocation.Y), DeltaTime, MovementLagSpeed);
	return FVector(TickDesiredLocation.X, TickDesiredLocation.Y, CurrentLocation.Z);
}

FVector AUniversalCamera::Lag_GetTickDesiredSocketOffset_Implementation(float DeltaTime)
{
	FVector CurrentOffset = SpringArmComponent->SocketOffset;
	FVector TickDesiredOffset = FMath::VInterpTo(CurrentOffset, DesiredSocketOffset, DeltaTime, OffsetLagSpeed);
	return TickDesiredOffset;
}

FVector AUniversalCamera::Lag_GetTickDesiredTargetOffset_Implementation(float DeltaTime)
{
	FVector CurrentOffset = SpringArmComponent->TargetOffset;
	FVector TickDesiredOffset = FMath::VInterpTo(CurrentOffset, DesiredTargetOffset, DeltaTime, OffsetLagSpeed);
	return TickDesiredOffset;
}

float AUniversalCamera::Lag_GetTickDesiredHeight_Implementation(float DeltaTime)
{
	float TickDesiredHeight = FMath::FInterpTo(GetActorLocation().Z, DesiredLocation.Z, DeltaTime, HeightLagSpeed);
	return TickDesiredHeight;
}

FRotator AUniversalCamera::Lag_GetTickDesiredRotation_Implementation(float DeltaTime)
{
	FRotator TickDesiredRotation = FRotator(FMath::QInterpTo(FQuat(GetActorRotation()), FQuat(GetDesiredRotationWithOffset()), DeltaTime, RotationLagSpeed));
	return TickDesiredRotation;
}

float AUniversalCamera::Lag_GetTickDesiredZoom_Implementation(float DeltaTime)
{
	float TickDesiredZoom = FMath::FInterpTo(SpringArmComponent->TargetArmLength, DesiredZoom, DeltaTime, ZoomLagSpeed);
	return TickDesiredZoom;
}

float AUniversalCamera::GetScaledValue(float AxisValue, float Multiplier, float Speed, float ScalingValue, float Min, float Max) const
{
	return AxisValue * Multiplier * (Max > 0 ? FMath::Clamp(Speed * ScalingValue, Min, Max) : FMath::Max(Speed * ScalingValue, Min));
}

void AUniversalCamera::MoveForward(float AxisValue, float Multiplier, bool MoveTargetOffset, bool MoveSocketOffset)
{
	if (IsFollowingAnyActor() && !(MoveSocketOffset || MoveTargetOffset)) return;
	if (!(EnabledFeatures & EEnabledFeatures::Movement)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	FVector ForwardVector = UseAlternativeForwardVector ? GetAlternativeForwardVector(DesiredRotation) : UKismetMathLibrary::GetForwardVector(DesiredRotation);

	if (MoveSocketOffset)
	{
		ForwardVector = DesiredRotation.UnrotateVector(ForwardVector);

		if (ApplyOffsetScaling) SetDesiredSocketOffset(DesiredSocketOffset + ForwardVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredSocketOffset(DesiredSocketOffset + ForwardVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else if (MoveTargetOffset)
	{
		if (ApplyOffsetScaling) SetDesiredTargetOffset(DesiredTargetOffset + ForwardVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredTargetOffset(DesiredTargetOffset + ForwardVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else
	{
		if (ApplyMovementScaling) SetDesiredLocation(DesiredLocation + ForwardVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, MovementSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredLocation(DesiredLocation + ForwardVector * GetTickMultiplier() * AxisValue * Multiplier * MovementSpeed);
	}
}

void AUniversalCamera::MoveRight(float AxisValue, float Multiplier, bool MoveTargetOffset, bool MoveSocketOffset)
{
	if (IsFollowingAnyActor() && !(MoveSocketOffset || MoveTargetOffset)) return;
	if (!(EnabledFeatures & EEnabledFeatures::Movement)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	FVector RightVector = UseAlternativeRightVector ? GetAlternativeRightVector(DesiredRotation) : UKismetMathLibrary::GetRightVector(DesiredRotation);

	if (MoveSocketOffset)
	{
		RightVector = FVector(0.f, 1.f, 0.f);
		if (ApplyOffsetScaling) SetDesiredSocketOffset(DesiredSocketOffset + RightVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredSocketOffset(DesiredSocketOffset + RightVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else if (MoveTargetOffset)
	{
		if (ApplyOffsetScaling) SetDesiredTargetOffset(DesiredTargetOffset + RightVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredTargetOffset(DesiredTargetOffset + RightVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else
	{
		if (ApplyMovementScaling) SetDesiredLocation(DesiredLocation + RightVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, MovementSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredLocation(DesiredLocation + RightVector * GetTickMultiplier() * AxisValue * Multiplier * MovementSpeed);
	}
}

void AUniversalCamera::MoveUp(float AxisValue, float Multiplier, bool MoveTargetOffset, bool MoveSocketOffset)
{
	if (IsFollowingAnyActor() && !(MoveSocketOffset || MoveTargetOffset)) return;
	if (!(EnabledFeatures & EEnabledFeatures::Movement)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	FVector UpVector = UseAlternativeUpVector ? GetAlternativeUpVector(DesiredRotation) : UKismetMathLibrary::GetUpVector(DesiredRotation);

	if (MoveSocketOffset)
	{
		UpVector = DesiredRotation.UnrotateVector(UpVector);

		if (ApplyOffsetScaling) SetDesiredSocketOffset(DesiredSocketOffset + UpVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredSocketOffset(DesiredSocketOffset + UpVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else if (MoveTargetOffset)
	{
		if (ApplyOffsetScaling) SetDesiredTargetOffset(DesiredTargetOffset + UpVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, OffsetSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredTargetOffset(DesiredTargetOffset + UpVector * GetTickMultiplier() * AxisValue * Multiplier * OffsetSpeed);
	}
	else
	{
		if (ApplyMoveUpScaling) SetDesiredLocation(DesiredLocation + UpVector * GetTickMultiplier() * GetScaledValue(AxisValue, Multiplier, MovementSpeed, GetMovementScaling(), MinimumMovementSpeed, MaximumMovementSpeed));
		else SetDesiredLocation(DesiredLocation + UpVector * GetTickMultiplier() * AxisValue * Multiplier * MovementSpeed);
	}
}

void AUniversalCamera::Move(float ForwardAxis, float RightAxis, float UpAxis, float Multiplier, bool NormalizeDirection, bool MoveTargetOffset, bool MoveSocketOffset)
{
	FVector AxisDirection = FVector(ForwardAxis, RightAxis, UpAxis);
	if (NormalizeDirection) AxisDirection.Normalize();
	MoveForward(AxisDirection.X, Multiplier, MoveTargetOffset, MoveSocketOffset);
	MoveRight(AxisDirection.Y, Multiplier, MoveTargetOffset, MoveSocketOffset);
	MoveUp(AxisDirection.Z, Multiplier, MoveTargetOffset, MoveSocketOffset);
}

void AUniversalCamera::RotateYaw(float AxisValue, float Multiplier, bool RotateOffset)
{
	if (!(EnabledFeatures & EEnabledFeatures::Rotation)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	float NewYawOffset = DesiredRotationOffset.Yaw - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;
	float NewYaw = DesiredRotation.Yaw - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;
	if (RotateOffset)
	{
		SetDesiredYawOffset(NewYawOffset);
	}
	else
	{
		SetDesiredYaw(NewYaw);
	}
}

void AUniversalCamera::RotatePitch(float AxisValue, float Multiplier, bool RotateOffset)
{
	if (!(EnabledFeatures & EEnabledFeatures::Rotation)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	float NewPitchOffset = DesiredRotationOffset.Pitch - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;
	float NewPitch = DesiredRotation.Pitch - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;

	if (RotateOffset)
	{
		SetDesiredPitchOffset(NewPitchOffset);
	}
	else
	{
		SetDesiredPitch(NewPitch);
	}
	
}

void AUniversalCamera::RotateRoll(float AxisValue, float Multiplier, bool RotateOffset)
{
	if (!(EnabledFeatures & EEnabledFeatures::Rotation)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	float NewRollOffset = DesiredRotationOffset.Roll - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;
	float NewRoll = DesiredRotation.Roll - GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier;
	
	if (RotateOffset)
	{
		SetDesiredRollOffset(NewRollOffset);
	}
	else
	{
		SetDesiredRoll(NewRoll);
	}
	
}

float AUniversalCamera::GetZoomInValue(float AxisValue, float Multiplier)
{
	return UseZoomScaling ? GetScaledValue(AxisValue, Multiplier, ZoomSpeed, GetZoomInScaling(), MinimumZoomSpeed, MaximumZoomSpeed) * GetTickMultiplier() :
							AxisValue * Multiplier * ZoomSpeed * GetTickMultiplier();
}

float AUniversalCamera::GetZoomInValueFromDistance(float AxisValue, float Multiplier, float Distance)
{
	return UseZoomScaling ? GetScaledValue(AxisValue, Multiplier, ZoomSpeed, Distance / ZoomReference, MinimumZoomSpeed, MaximumZoomSpeed) * GetTickMultiplier() :
							AxisValue * Multiplier * ZoomSpeed * GetTickMultiplier();
}

void AUniversalCamera::ZoomIn(float AxisValue, float Multiplier)
{
	if (!(EnabledFeatures & EEnabledFeatures::Zoom)) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	SetDesiredZoom(DesiredZoom - GetZoomInValue(AxisValue, Multiplier));
}

float AUniversalCamera::GetMovementScaling_Implementation()
{
	if (ScalingTraceToGround)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsValid(PlayerController)) return 0.f;

		FCollisionObjectQueryParams Params;
		for (ECollisionChannel Channel : TerrainHeightAdaptationChannels) { Params.AddObjectTypesToQuery(Channel); }

		FHitResult HitResult;
		FVector Start = DesiredLocation;
		FVector End = DesiredLocation - FVector(0.0f, 0.0f, PlayerController->HitResultTraceDistance);
		bool DidHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, Params);
		if (DidHit)
		{
			GroundLevel = HitResult.ImpactPoint.Z;
		}
	}
	return UseZoomLevel ? DesiredZoom / ScalingDistanceReference : (DesiredLocation.Z - GroundLevel) / ScalingDistanceReference;
}

float AUniversalCamera::GetZoomInScaling_Implementation()
{
	return DesiredZoom / ZoomReference;
}

FVector AUniversalCamera::GetCorrectedDestinationFromOriginCollisions(FVector Location, FVector Destination, bool& DidHit)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(Origin), false, GetOwner());
	GetWorld()->SweepSingleByChannel(HitResult, Location, Destination, FQuat::Identity, OriginCollisionsChannel, FCollisionShape::MakeSphere(OriginCollisionsProbeSize), QueryParams);
	if (HitResult.bBlockingHit)
	{
		FVector BA = Destination - HitResult.Location;
		FVector AH = HitResult.ImpactNormal;

		float BALength = BA.Length();
		BA.Normalize();

		Destination += HitResult.ImpactNormal * (FMath::Abs(FVector::DotProduct(BA, AH) * BALength) + 1.f);
	}
	else return Destination;

	GetWorld()->SweepSingleByChannel(HitResult, Location, Destination, FQuat::Identity, OriginCollisionsChannel, FCollisionShape::MakeSphere(OriginCollisionsProbeSize), QueryParams);
	if (HitResult.bBlockingHit)
	{
		return HitResult.Location + HitResult.ImpactNormal;
	}
	else return Destination;

}

float AUniversalCamera::GetTerrainHeightAdaptationValue_Implementation(bool& IsValidLoc)
{
	IsValidLoc = false;
	float HeightValue = DesiredLocation.Z;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return HeightValue;

	if (TerrainHeightAdaptationChannels.Num() == 0) return HeightValue;

	FHitResult HitResult;
	float ControllerTraceDistance = PlayerController->HitResultTraceDistance;
	FVector Start = CanGoBelowSurfaces ? DesiredLocation + FVector(0.0f, 0.0f, GetDesiredCameraWorldLocation().Z) : DesiredLocation + FVector(0.0f, 0.0f, ControllerTraceDistance);
	FVector End = DesiredLocation - FVector(0, 0, ControllerTraceDistance);
	
	FCollisionObjectQueryParams Params;
	for (ECollisionChannel Channel : TerrainHeightAdaptationChannels) { Params.AddObjectTypesToQuery(Channel); }

	//	Check under and above Origin
	bool DidHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, Params, FCollisionShape::MakeSphere(ZProbeRadius));
	if (DidHit)
	{
		if (DistanceAdaptation && HitResult.ImpactPoint.Z <= GetDesiredCameraWorldLocation().Z)
		{
			if (DistanceBypassValue < DesiredZoom) return HeightValue;
		}
		
		if (DesiredLocation.Z - HitResult.ImpactPoint.Z != KINDA_SMALL_NUMBER)
		{
			IsValidLoc = true;
			HeightValue = HitResult.ImpactPoint.Z + OriginDistanceFromGround;
		}

		//	Check if camera has vision on origin if asked
		if (AlwaysSeeOrigin)
		{
			DidHit = GetWorld()->SweepSingleByObjectType(HitResult, GetDesiredCameraWorldLocation(), DesiredLocation, FQuat::Identity, Params, FCollisionShape::MakeSphere(ZProbeRadius));
			if (DidHit)
			{
				IsValidLoc = true;
				HeightValue = HitResult.ImpactPoint.Z + OriginDistanceFromGround;
			}
		}
	}
	return HeightValue;
}

void AUniversalCamera::ApplyTerrainHeightAdaptationTick()
{
	bool IsValidLoc = false;
	float NewZLoc = GetTerrainHeightAdaptationValue(IsValidLoc);

	if (IsValidLoc)
	{
		FVector NewLocation = DesiredLocation;
		NewLocation.Z = NewZLoc;
		SetDesiredLocation(NewLocation);

		//	Update the MouseDragOrigin
		PreciseMouseDragOrigin.Z = DesiredLocation.Z;
	}
}

FVector2D AUniversalCamera::GetMousePositionRatio(APlayerController* PlayerController, FVector2D& RawPosition)
{
	if (!IsValid(PlayerController)) return FVector2D(0.f, 0.f);
	
	float OriginX, OriginY;
	PlayerController->GetMousePosition(OriginX, OriginY);
	FVector2D Origin(OriginX, OriginY);
	RawPosition = Origin;

	int32 SizeX, SizeY;
	PlayerController->GetViewportSize(SizeX, SizeY);
	FVector2D Size(SizeX, SizeY);

	return Origin / Size;
}

FVector AUniversalCamera::GetImpactPointUnderCursor(APlayerController* PlayerController, ECollisionChannel TraceChannel, bool TraceComplex, bool& DidHit)
{
	DidHit = false;
	if (!IsValid(PlayerController)) return FVector(0.f, 0.f, 0.f);

	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(TraceChannel, TraceComplex, HitResult);
	DidHit = HitResult.IsValidBlockingHit();
	if (DidHit) return HitResult.ImpactPoint;
	else return FVector(0.f, 0.f, 0.f);
}

void AUniversalCamera::ToggleScreenSliding(bool Activate)
{
	if (Activate && EnabledFeatures & ScreenSliding)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsValid(PlayerController)) return;

		FVector2D RawPosition(0.f, 0.f);
		ScreenSlidingOriginRatio = GetMousePositionRatio(PlayerController, RawPosition);
		IsScreenSliding = true;

		OnBeginScreenSliding(RawPosition);
	}
	else
	{
		IsScreenSliding = false;

		OnEndScreenSliding();
	}
}

void AUniversalCamera::ScreenSlidingTick()
{
	if (ScreenSlidingType == ScreenSliding_None) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	FVector2D RawPosition;
	FVector2D CurrentMousePositionRatio = GetMousePositionRatio(PlayerController, RawPosition);
	FVector2D Delta = CurrentMousePositionRatio - ScreenSlidingOriginRatio;

	float StrengthX = FMath::Min(FMath::Abs(Delta.X), ScreenSlidingMaxRatio) * ScreenSlidingMaxSpeed / ScreenSlidingMaxRatio;
	float StrengthY = FMath::Min(FMath::Abs(Delta.Y), ScreenSlidingMaxRatio) * ScreenSlidingMaxSpeed / ScreenSlidingMaxRatio;

	FVector2D Direction = CurrentMousePositionRatio - ScreenSlidingOriginRatio;
	Direction.Normalize();
	OnUpdateScreenSliding(Direction);

	float AxisX = Delta.X > 0 ? 1.0f : -1.0f;
	if (ScreenSlidingInvertXAxis) AxisX *= -1.0f;
	float AxisY = Delta.Y > 0 ? -1.0f : 1.0f;
	if (ScreenSlidingInvertYAxis) AxisY *= -1.0f;

	switch (ScreenSlidingType)
	{
	case SlideCamera:
		MoveRight(AxisX, StrengthX);
		MoveForward(AxisY, StrengthY);
		return;
	case SlideSocketOffset:
		MoveRight(AxisX, StrengthX, true, false);
		MoveForward(AxisY, StrengthY, true, false);
		return;
	case SlideTargetOffset:
		MoveRight(AxisX, StrengthX, false, true);
		MoveForward(AxisY, StrengthY, false, true);
		return;
	case SlideYaw:
		RotateYaw(AxisX, StrengthX);
		return;
	case SlidePitch:
		RotatePitch(AxisY, StrengthY);
		return;
	case SlideYawAndPitch:
		RotateYaw(AxisX, StrengthX);
		RotatePitch(AxisY, StrengthY);
		return;
	};
}

void AUniversalCamera::FollowTarget(FTargetSettings_WithTemplates TargetSettings, FConstrainVector2 FollowAxis, FBoolRotation FollowRotations, bool ResetOffsetIfPrematurelyEnded)
{
	if (!(EnabledFeatures & EEnabledFeatures::FollowTarget)) return;
	if (!FollowAxis.IsValid()) return;

	switch (TargetSettings.Type)
	{
	case 0:
		if (!TargetSettings.TargetSettings.IsValidActor()) return;
		break;
	case 1:
		if (!TargetSettings.TargetSettings.IsValidSceneComponent()) return;
		break;
	case 2:
		if (!TargetSettings.TargetSettings.IsValidSocket()) return;
		break;
	}

	if (IsTraveling && IsValid(CurrentTravelTask)) EndTravelTask(false);

	FollowTargetSettings = TargetSettings;
	FollowedAxis = FollowAxis;
	FollowedRotations = FollowRotations;
	bShouldResetOffsetIfFailed = ResetOffsetIfPrematurelyEnded;
	bIsFollowingAnyActor = true;
}

void AUniversalCamera::FollowTargetTick()
{
	bool IsValidTarget;
	FRotator TargetRotation;
	FVector TargetLocation = FollowTargetSettings.GetTargetLocation(IsValidTarget, TargetRotation);

	if (!IsValidTarget)
	{
		StopFollowing_Internal(bShouldResetOffsetIfFailed, StopFollowingReason_InvalidTarget);
		return;
	}
		
	FVector NewDesiredLocation = DesiredLocation;

	if (FollowedAxis.X) NewDesiredLocation.X = TargetLocation.X;
	if (FollowedAxis.Y) NewDesiredLocation.Y = TargetLocation.Y;
	if (FollowedAxis.Z) NewDesiredLocation.Z = TargetLocation.Z;

	SetDesiredLocation(NewDesiredLocation, false, bFollowActorIgnoreRestrictions);

	FRotator NewDesiredRotation = DesiredRotation;

	if (FollowedRotations.Yaw) NewDesiredRotation.Yaw = TargetRotation.Yaw;
	if (FollowedRotations.Pitch) NewDesiredRotation.Pitch = TargetRotation.Pitch;
	if (FollowedRotations.Roll) NewDesiredRotation.Roll = TargetRotation.Roll;

	SetDesiredRotation(NewDesiredRotation, false, true);
}

void AUniversalCamera::StopFollowing_Internal(bool bResetOffset, EStopFollowingReason Reason)
{
	bIsFollowingAnyActor = false;
	FollowTargetSettings.Empty();

	OnStoppedFollowing(Reason);

		if (bResetOffset)
		{
			ResetSocketOffset();
			ResetTargetOffset();
			ResetRotationOffset();
		}
}

FVector AUniversalCamera::GetCorrectedDestinationFromDotsShape(const FVector& Destination, const TArray<FVector2D>& PolygonPoints)
{
	// Convert the 3D destination point to a 2D point
	FVector2D Destination2D(Destination.X, Destination.Y);

	// Check if the destination point is inside the polygon
	if (IsPointInPolygon(Destination2D, PolygonPoints))
	{
		// If the destination point is inside the polygon, return it as is
		return Destination;
	}
	else
	{
		// If the destination point is outside the polygon, find the closest point on the polygon
		FVector2D ClosestPoint = GetClosestPointOnPolygon(Destination2D, PolygonPoints);

		// Set the Z coordinate of the corrected destination to the original Z coordinate of the destination
		FVector CorrectedDestination(ClosestPoint.X, ClosestPoint.Y, Destination.Z);

		return CorrectedDestination;
	}
}

bool AUniversalCamera::IsPointInPolygon(const FVector2D& Point, const TArray<FVector2D>& PolygonPoints)
{
	int32 NumVertices = PolygonPoints.Num();
	bool Inside = false;
	for (int32 i = 0, j = NumVertices - 1; i < NumVertices; j = i++)
	{
		if (((PolygonPoints[i].Y > Point.Y) != (PolygonPoints[j].Y > Point.Y)) &&
			(Point.X < (PolygonPoints[j].X - PolygonPoints[i].X) * (Point.Y - PolygonPoints[i].Y) / (PolygonPoints[j].Y - PolygonPoints[i].Y) + PolygonPoints[i].X))
		{
			Inside = !Inside;
		}
	}
	return Inside;
}

FVector2D AUniversalCamera::GetClosestPointOnPolygon(const FVector2D& Point, const TArray<FVector2D>& PolygonPoints)
{
	FVector2D ClosestPoint;
	float ClosestDistanceSq = TNumericLimits<float>::Max();

	for (int32 i = 0; i < PolygonPoints.Num(); i++)
	{
		FVector2D EdgeStart = PolygonPoints[i];
		FVector2D EdgeEnd = PolygonPoints[(i + 1) % PolygonPoints.Num()];
		FVector2D EdgeDirection = (EdgeEnd - EdgeStart).GetSafeNormal();
		float DistanceAlongEdge = FVector2D::DotProduct(Point - EdgeStart, EdgeDirection);
		FVector2D ClosestPointOnEdge = EdgeStart + EdgeDirection * FMath::Clamp(DistanceAlongEdge, 0.f, (EdgeEnd - EdgeStart).Size());
		float DistanceSq = (Point - ClosestPointOnEdge).SizeSquared();
		if (DistanceSq < ClosestDistanceSq)
		{
			ClosestPoint = ClosestPointOnEdge;
			ClosestDistanceSq = DistanceSq;
		}
	}

	return ClosestPoint;
}

FVector AUniversalCamera::GetCorrectedDestinationFromRestrictions(FVector Destination)
{
	Destination = GetClampedLocation(Destination);

	if (ConstrainLocationToShape)
	{
		Destination = GetCorrectedDestinationFromDotsShape(Destination, ConstrainDots);
	}

	return Destination;
}

TArray<FVector2D> AUniversalCamera::GetActorsLocation2D(TArray<AActor*> Actors)
{
	TArray<FVector2D> ReturnActors;
	for (AActor*& Actor : Actors)
	{
		FVector Location = Actor->GetActorLocation();
		ReturnActors.Add(FVector2D(Location.X, Location.Y));
	}
	return ReturnActors;
}

TArray<uint8> AUniversalCamera::GetSettingsSaveFormat()
{
	TArray<uint8> ObjectData;
	FMemoryWriter MemoryWriter(ObjectData, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
	Ar.ArNoDelta = true;
	this->Serialize(Ar);
	return ObjectData;
}

void AUniversalCamera::LoadSettingsSaveFormat(TArray<uint8> SettingsSaveFormat)
{
	FMemoryReader MemoryReader(SettingsSaveFormat, true);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, false);
	Ar.ArIsSaveGame = true;
	Ar.ArNoDelta = true;
	this->Serialize(Ar);
}

FUniversalCameraPositionSaveFormat AUniversalCamera::GetPositionSaveFormat()
{
	FUniversalCameraPositionSaveFormat SaveFormat;
	SaveFormat.DesiredLocation = DesiredLocation;
	SaveFormat.DesiredSocketOffset = DesiredSocketOffset;
	SaveFormat.DesiredTargetOffset = DesiredTargetOffset;
	SaveFormat.DesiredRotation = DesiredRotation;
	SaveFormat.DesiredZoom = DesiredZoom;

	return SaveFormat;
}

void AUniversalCamera::LoadPositionSaveFormat(FUniversalCameraPositionSaveFormat PositionSaveFormat)
{
	SetDesiredPosition(PositionSaveFormat.DesiredLocation, PositionSaveFormat.DesiredSocketOffset, PositionSaveFormat.DesiredTargetOffset, PositionSaveFormat.DesiredRotation.Yaw, PositionSaveFormat.DesiredRotation.Pitch, PositionSaveFormat.DesiredRotation.Roll, PositionSaveFormat.DesiredZoom, true, true);
}

void AUniversalCamera::InitUCSaveGameInstance()
{
	if (!IsValid(UCSaveGameInstance))
	{
		UCSaveGameInstance = Cast<UUniversalCameraSaveGame>(UGameplayStatics::CreateSaveGameObject(UUniversalCameraSaveGame::StaticClass()));
	}
}

void AUniversalCamera::QuickSavePosition(FUniversalCameraPositionSaveFormat PositionSaveFormat, const FString SlotName, const int32 UserIndex)
{
	InitUCSaveGameInstance();

	UCSaveGameInstance->SavedPosition = PositionSaveFormat;
	UCSaveGameInstance->bIsValidSavePosition = true;
	UGameplayStatics::SaveGameToSlot(UCSaveGameInstance, SlotName, UserIndex);
}

void AUniversalCamera::QuickLoadPosition(const FString SlotName, const int32 UserIndex)
{
	UUniversalCameraSaveGame* UCSave = Cast<UUniversalCameraSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if (IsValid(UCSave) && UCSave->bIsValidSavePosition)
	{
		LoadPositionSaveFormat(UCSave->SavedPosition);
	}
}

void AUniversalCamera::QuickSaveSettings(TArray<uint8> SettingsSaveFormat, const FString SlotName, const int32 UserIndex)
{
	InitUCSaveGameInstance();

	UCSaveGameInstance->SavedSettings = SettingsSaveFormat;
	UCSaveGameInstance->bIsValidSaveSettings = true;
	UGameplayStatics::SaveGameToSlot(UCSaveGameInstance, SlotName, UserIndex);
}

void AUniversalCamera::QuickLoadSettings(const FString SlotName, const int32 UserIndex)
{
	UUniversalCameraSaveGame* UCSave = Cast<UUniversalCameraSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));

	if (IsValid(UCSave) && UCSave->bIsValidSaveSettings)
	{
		LoadSettingsSaveFormat(UCSave->SavedSettings);
	}
}

bool AUniversalCamera::GetActorFocusLocation_Implementation(AActor* Actor, FVector& Location, float& IdealZoom)
{
	if (!Actor) return false;

	FVector Origin, BoxExtent;
	Actor->GetActorBounds(false, Origin, BoxExtent);
	Location = Origin;

	float Radius = BoxExtent.Size();
	IdealZoom = Radius * 2.0f;

	return true;
}

FVector AUniversalCamera::FindVectorAtZ(bool& Intersect, FVector Direction, float Z)
{
	Intersect = false;
	if (Direction.Z == 0.f) return FVector(0.f, 0.f, 0.f);

	Intersect = true;

	Direction.Normalize();
	float Multiplier = Z / Direction.Z;
	return Direction * Multiplier;
}

void AUniversalCamera::ZoomAtLocation(float AxisValue, FVector Location, float Multiplier, bool LockZoomLevel)
{
	if (AxisValue == 0.f) return;

	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);

	FVector Direction = Location - GetDesiredCameraWorldLocation();
	Direction.Normalize();
	float ScalingDistance = (Location - DesiredLocation).Length();

	if (LockZoomLevel)
	{
		SetDesiredLocation(DesiredLocation + GetZoomInValueFromDistance(AxisValue, Multiplier, ScalingDistance) * Direction);
		return;
	}
	else
	{
		ZoomIn(AxisValue, Multiplier);
	}
	

	if (Direction.Z > -0.1f) return;

	float ZDirection = GetDesiredCameraWorldLocation().Z - Location.Z;

	bool Intersect;

	FVector NewCameraLoc = Location + FindVectorAtZ(Intersect, Direction, ZDirection);

	if (Intersect)
	{
		FVector NewOriginDesiredLoc = GetOriginLocationFromCameraLocation(NewCameraLoc);
		SetDesiredLocation(NewOriginDesiredLoc);
	}
}

void AUniversalCamera::ZoomAtCursor(float AxisValue, ECollisionChannel CollisionChannel, bool& DidHit, float Multiplier, bool LockZoomLevel)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	FVector ImpactPoint = GetImpactPointUnderCursor(PlayerController, CollisionChannel, false, DidHit);

	if (DidHit)
	{
		ZoomAtLocation(AxisValue, ImpactPoint, Multiplier, LockZoomLevel);
	}
	else
	{
		ZoomIn(AxisValue, Multiplier);
	}
}

void AUniversalCamera::TogglePreciseMouseDrag(bool Activate)
{
	if (Activate && (EnabledFeatures & EEnabledFeatures::PreciseMouseDrag))
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsValid(PlayerController)) return;
		
		bool DidHit = false;
		
		if (IsValid(PlayerController))
		{
			PreciseMouseDragOrigin = GetImpactPointUnderCursor(PlayerController, PreciseMouseDragCollisionChannel, PreciseMouseDragTraceComplex, DidHit);
		}
		IsPreciseMouseDragging = DidHit;
	}
	else
	{
		IsPreciseMouseDragging = false;
	}
}

void AUniversalCamera::PreciseMouseDragTick()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) return;

	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	FVector WorldPosition;
	FVector Direction;
	UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldPosition, Direction);

	FVector P = WorldPosition + (DesiredLocation - GetActorLocation());
	FVector D = Direction;
	FVector M;

	M.Z = PreciseMouseDragOrigin.Z;
	M.X = P.X + (PreciseMouseDragOrigin.Z - P.Z) * D.X / D.Z;
	M.Y = P.Y + (PreciseMouseDragOrigin.Z - P.Z) * D.Y / D.Z;
	
	FVector NewLocation = DesiredLocation - (M - PreciseMouseDragOrigin);

	SetDesiredLocation(NewLocation, PreciseMouseDragIgnoreMovementLag);
}

void AUniversalCamera::RotateYawAroundPivot(FVector Pivot, float AxisValue, float Multiplier)
{
	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);
	FVector Direction = DesiredLocation - Pivot;
	float OldYaw = DesiredRotation.Yaw;
	SetDesiredYaw(DesiredRotation.Yaw + GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier);
	float Delta = DesiredRotation.Yaw - OldYaw;
	Direction = Direction.RotateAngleAxis(Delta, FVector(0.0f, 0.0f, 1.0f));
	SetDesiredLocation(Pivot + Direction);
}

void AUniversalCamera::RotatePitchAroundPivot(FVector Pivot, float AxisValue, float Multiplier, bool LockZAxis)
{
	AxisValue = FMath::Clamp(AxisValue, -1.f, 1.f);
	FVector Direction = DesiredLocation - Pivot;
	float OldPitch = DesiredRotation.Pitch;
	SetDesiredPitch(DesiredRotation.Pitch + GetTickMultiplier() * AxisValue * RotateSpeed * Multiplier);
	float Delta =  OldPitch - DesiredRotation.Pitch;
	Direction = Direction.RotateAngleAxis(Delta, UKismetMathLibrary::GetRightVector(DesiredRotation));

	float DesiredZ = DesiredLocation.Z;

	FVector NewOriginLocation = Pivot + Direction;
	if (LockZAxis)
	{
		if ((DesiredRotation.Pitch < 8.f && DesiredRotation.Pitch > -8.f) || DesiredRotation.Pitch > 172.f || DesiredRotation.Pitch < -172.f) return;

		float DirectionMultiplier = (DesiredZ - NewOriginLocation.Z) / DesiredRotation.Vector().Z;
		NewOriginLocation = NewOriginLocation + DesiredRotation.Vector() * DirectionMultiplier;

		SetDesiredZoom(DesiredZoom + DirectionMultiplier);
	}
	SetDesiredLocation(NewOriginLocation, false, true);
}

void AUniversalCamera::PrintDesiredPosition(const float Duration)
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	GEngine->AddOnScreenDebugMessage(0, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Location: %s"), *DesiredLocation.ToString()));
	GEngine->AddOnScreenDebugMessage(1, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Yaw: %f + %f"), DesiredRotation.Yaw, DesiredRotationOffset.Yaw));
	GEngine->AddOnScreenDebugMessage(2, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Pitch: %f + %f"), DesiredRotation.Pitch, DesiredRotationOffset.Pitch));
	GEngine->AddOnScreenDebugMessage(3, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Roll: %f + %f"), DesiredRotation.Roll, DesiredRotationOffset.Roll));
	GEngine->AddOnScreenDebugMessage(4, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Zoom: %f"), DesiredZoom));
	GEngine->AddOnScreenDebugMessage(5, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Socket offset: %s"), *DesiredSocketOffset.ToString()));
	GEngine->AddOnScreenDebugMessage(6, Duration, FColor::Yellow, FString::Printf(TEXT("Desired Target offset: %s"), *DesiredTargetOffset.ToString()));
#endif
}

FVector AUniversalCamera::GetLinePlaneIntersection(FVector Direction, FVector Origin, FVector PlaneNormal, FVector PlaneCoord, bool& Intersect)
{
	float d = PlaneNormal.Dot(PlaneCoord);
	if (PlaneNormal.Dot(Direction) == 0) {
		Intersect = false;
		return FVector(0.f, 0.f, 0.f); // No intersection, the line is parallel to the plane
	}

	float x = (d - PlaneNormal.Dot(Origin)) / PlaneNormal.Dot(Direction);
	// return contact point
	Direction.Normalize();
	FVector Contact = Origin + Direction * x;
	Intersect = true;
	return Contact;
}

FVector AUniversalCamera::GetAlternativeForwardVector(FRotator Rotation)
{
	Rotation.Pitch = 0.f;
	Rotation.Roll = 0.f;
	return UKismetMathLibrary::GetForwardVector(Rotation);
}

FVector AUniversalCamera::GetAlternativeRightVector(FRotator Rotation)
{
	Rotation.Pitch = 0.f;
	Rotation.Roll = 0.f;
	return UKismetMathLibrary::GetRightVector(Rotation);
}

FVector AUniversalCamera::GetAlternativeUpVector(FRotator Rotation)
{
	return FVector::UpVector;
}
