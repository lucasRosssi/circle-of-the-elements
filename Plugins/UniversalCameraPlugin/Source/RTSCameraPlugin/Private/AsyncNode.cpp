// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNode.h"
#include "UniversalCamera.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

//	----------------------------	ASYNC NODE	---------------------------------------

UAsyncNode* UAsyncNode::CameraTravel(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, FTargetVector LocationSettings, FOffsetSettings OffsetSettings, FTargetFloat YawSettings, FTargetFloat PitchSettings, FTargetFloat RollSettings, FTargetFloat ZoomSettings, UCurveFloat* Curve, FTravelSpeedSettings SpeedSettings, bool LockAllMovement, bool IgnoreLag, bool IgnoreRestrictions)
{
	if (LocationSettings.TargetMod == TargetMod_None && OffsetSettings.OffsetMod == 0 && YawSettings.TargetMod == TargetMod_None && PitchSettings.TargetMod == TargetMod_None && RollSettings.TargetMod == TargetMod_None && ZoomSettings.TargetMod == TargetMod_None) return nullptr;

	//	Calculate Location Direction -------------------------------------------------

	FVector StartingLocation, LocationDirection;

	bool IsValidLocation = false;

	if (LocationSettings.TargetMod == ETargetMod::TargetMod_Spline)
	{
		IsValidLocation = IsValid(LocationSettings.SplineComponent);
	}
	else
	{
		StartingLocation = UniversalCamera->DesiredLocation;
		LocationDirection = LocationSettings.GetDirection(UniversalCamera, TargetSettings, StartingLocation, IgnoreRestrictions, IsValidLocation);
	}

	//	Offset -----------------------------------------------------------

	bool IsValidOffset = false;
	FVector StartingOffset = FVector(0.f, 0.f, 0.f);
	switch (OffsetSettings.OffsetMod)
	{
	case 1:
		StartingOffset = UniversalCamera->DesiredTargetOffset;
		break;
	case 2:
		StartingOffset = UniversalCamera->DesiredSocketOffset;
		break;
	};
	FVector OffsetDirection = OffsetSettings.GetDirection(UniversalCamera, IgnoreRestrictions, IsValidOffset);

	//	Yaw -----------------------------------------------------------

	bool IsValidYaw = false;
	float StartingYaw = UniversalCamera->DesiredRotation.Yaw;
	float YawDirection = YawSettings.GetDirection(UniversalCamera, 0, TargetSettings, StartingYaw, IgnoreRestrictions, IsValidYaw);

	//	Pitch ---------------------------------------------------------

	bool IsValidPitch = false;
	float StartingPitch = UniversalCamera->DesiredRotation.Pitch;
	float PitchDirection = PitchSettings.GetDirection(UniversalCamera, 1, TargetSettings, StartingPitch, IgnoreRestrictions, IsValidPitch);

	//	Roll -----------------------------------------------------------

	bool IsValidRoll = false;
	float StartingRoll = UniversalCamera->DesiredRotation.Roll;
	float RollDirection = RollSettings.GetDirection(UniversalCamera, 2, TargetSettings, StartingRoll, IgnoreRestrictions, IsValidRoll);

	//	Zoom --------------------------------------------------------

	bool IsValidZoom = false;
	float StartingZoom = UniversalCamera->DesiredZoom;
	float ZoomDirection = ZoomSettings.GetDirection(UniversalCamera, 3, TargetSettings, StartingZoom, IgnoreRestrictions, IsValidZoom);;

	//	Return if all settings are invalid
	if (!(IsValidLocation || IsValidOffset || IsValidYaw || IsValidPitch || IsValidRoll || IsValidZoom)) return nullptr;

	//	Calculate the duration from the speed -------------------

	float Duration = 0.f;
	if (!SpeedSettings.UseSpeed)
	{
		Duration = SpeedSettings.Duration;
	}

	float DurationTemp = 0.f;


	if (IsValidLocation)
	{
		if (SpeedSettings.UseSpeed)
		{
			if (LocationSettings.TargetMod == ETargetMod::TargetMod_Spline)
			{
				if (LocationSettings.SplineComponent)
				{
					Duration = LocationSettings.SplineComponent->GetSplineLength() / SpeedSettings.Speed;
				}
			}
			else
			{
				float LocationDistance = LocationDirection.Length();

				Duration = FMath::Abs(LocationDistance / SpeedSettings.Speed);
			}
			
		}
	}

	//	Calculate the farthest target (using the multiplier) and set it as the duration reference if using speed
	float OffsetLength = OffsetDirection.Length();
	SpeedSettings.DurationInitializer(IsValidOffset, OffsetLength, &Duration, 1.f);
	SpeedSettings.DurationInitializer(IsValidYaw, YawDirection, &Duration, 5.f);
	SpeedSettings.DurationInitializer(IsValidPitch, PitchDirection, &Duration, 5.f);
	SpeedSettings.DurationInitializer(IsValidRoll, RollDirection, &Duration, 5.f);
	SpeedSettings.DurationInitializer(IsValidZoom, ZoomDirection, &Duration, 3.f);


	//	Instantiate and initialize the Task ----------

	UAsyncNode* NewNode = NewObject<UAsyncNode>();

	if (!IsValid(NewNode)) return nullptr;

	//	Camera
	NewNode->m_UniversalCamera = UniversalCamera;
	NewNode->m_IgnoreLag = IgnoreLag;
	NewNode->m_IgnoreRestrictions = IgnoreRestrictions;
	//	Time
	NewNode->MaxDuration = Duration;
	//	Duration
	NewNode->TargetSettings = TargetSettings;
	//	Location
	NewNode->LocationSettings = LocationSettings;
	NewNode->StartingLocation = StartingLocation;
	NewNode->LocationDirection = LocationDirection;
	NewNode->IsValidLocation = IsValidLocation;
	//	Offset
	NewNode->OffsetSettings = OffsetSettings;
	NewNode->StartingOffset = StartingOffset;
	NewNode->OffsetDirection = OffsetDirection;
	NewNode->IsValidOffset = IsValidOffset;
	//	Yaw
	NewNode->YawSettings = YawSettings;
	NewNode->StartingYaw = StartingYaw;
	NewNode->YawDirection = YawDirection;
	NewNode->IsValidYaw = IsValidYaw;
	//	Pitch
	NewNode->PitchSettings = PitchSettings;
	NewNode->StartingPitch = StartingPitch;
	NewNode->PitchDirection = PitchDirection;
	NewNode->IsValidPitch = IsValidPitch;
	//	Roll
	NewNode->RollSettings = RollSettings;
	NewNode->StartingRoll = StartingRoll;
	NewNode->RollDirection = RollDirection;
	NewNode->IsValidRoll = IsValidRoll;
	//	Zoom
	NewNode->ZoomSettings = ZoomSettings;
	NewNode->StartingZoom = StartingZoom;
	NewNode->ZoomDirection = ZoomDirection;
	NewNode->IsValidZoom = IsValidZoom;
	//	Curve
	NewNode->m_Curve = Curve;
	//	Lock Movement
	NewNode->m_LockAllMovement = LockAllMovement;

	UniversalCamera->StartTraveling_Internal(NewNode);

	return NewNode;
}

void UAsyncNode::TryUpdateLocationDirectionAndValidity()
{
	switch (LocationSettings.TargetMod)
	{
	case TargetMod_Actor:
	case TargetMod_Socket:
	case TargetMod_SceneComponent:
		LocationDirection = GetLocationDirection(IsValidLocation);
		break;
	case TargetMod_Spline:
		IsValidLocation = IsValid(LocationSettings.SplineComponent);
		break;
	}
}

void UAsyncNode::TryUpdateFloatDirectionAndValidity(FTargetFloat* FloatSettings, float* FloatValue, DirectionFunction GetFloatDirection, bool* IsValidBool)
{
	switch (FloatSettings->TargetMod)
	{
	case TargetMod_Actor:
	case TargetMod_Socket:
	case TargetMod_SceneComponent:
		*FloatValue = (*this.*GetFloatDirection)(*IsValidBool);
	}
}

//	----------------------------	TRAVEL SPEED SETTINGS	---------------------------------------

void FTravelSpeedSettings::DurationInitializer(bool IsValidBool, float Direction, float* DurationRef, float Multiplier)
{
	if (IsValidBool)
	{
		if (UseSpeed)
		{
			float DurationTemp = FMath::Abs(Direction * Multiplier / Speed);
			if (DurationTemp > *DurationRef) *DurationRef = DurationTemp;
		}
		else
		{
			*DurationRef = Duration;
		}
	}
}

//	----------------------------	TARGET VECTOR	---------------------------------------

FVector FTargetVector::GetLocation(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, bool& IsValid)
{
	IsValid = false;

	if (UniversalCamera->IsFollowingAnyActor()) return FVector(0.f, 0.f, 0.f);

	switch (TargetMod)
	{
	case TargetMod_None:
		return FVector(0.f, 0.f, 0.f);
	case TargetMod_CustomValue:
		IsValid = true;
		return VectorValue;
	case TargetMod_Actor:
		IsValid = TargetSettings.IsValidActor();
		if (IsValid)
		{
			return TargetSettings.GetActorLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	case TargetMod_Socket:
		IsValid = TargetSettings.IsValidSocket();
		if (IsValid)
		{
			return TargetSettings.GetSocketLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	case TargetMod_SceneComponent:
		IsValid = TargetSettings.IsValidSceneComponent();
		if (IsValid)
		{
			return TargetSettings.GetSceneComponentLocation();
		}
		else return FVector(0.f, 0.f, 0.f);
	}

	return FVector(0.f, 0.f, 0.f);
}

FVector FTargetVector::GetDirection(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, FVector StartingLocation, bool IgnoreRestrictions, bool& IsValid)
{
	IsValid = false;
	FVector Location = GetLocation(UniversalCamera, TargetSettings, IsValid);
	if (!IgnoreRestrictions) Location = UniversalCamera->GetCorrectedDestinationFromRestrictions(Location);
	return Location - StartingLocation;
}

//	----------------------------	TARGET OFFSET	---------------------------------------

FVector FOffsetSettings::GetDirection(AUniversalCamera* UniversalCamera, bool IgnoreRestrictions, bool& IsValidOffset)
{
	IsValidOffset = false;
	FVector Offset = IgnoreRestrictions ? OffsetValue : UniversalCamera->GetClampedOffset(OffsetValue);

	switch (OffsetMod)
	{
	case 1:
		IsValidOffset = true;
		return Offset - UniversalCamera->DesiredTargetOffset;
	case 2:
		IsValidOffset = true;
		return Offset - UniversalCamera->DesiredSocketOffset;
	};

	return FVector(0.f, 0.f, 0.f);
}

//	----------------------------	TARGET FLOAT	---------------------------------------

float FTargetFloat::GetRotationValue(AUniversalCamera* UniversalCamera, int32 Type, FTargetSettings TargetSettings, bool IgnoreRestrictions, bool& IsValid)
{
	IsValid = false;
	FRotator RotatorValue(0.f, 0.f, 0.f);

	switch (TargetMod)
	{
	case TargetMod_None:
		return 0.f;
	case TargetMod_CustomValue:
		IsValid = true;
		return FloatValue;
	case TargetMod_Actor:
		IsValid = TargetSettings.IsValidActor();
		if (IsValid)
		{
			RotatorValue = TargetSettings.GetActorRotation();
			break;
		}
		else return 0.f;
	case TargetMod_Socket:
		IsValid = TargetSettings.IsValidSocket();
		if (IsValid)
		{
			RotatorValue = TargetSettings.GetSocketRotation();
			break;
		}
		else return 0.f;
	case TargetMod_SceneComponent:
		IsValid = TargetSettings.IsValidSceneComponent();
		if (IsValid)
		{
			RotatorValue = TargetSettings.GetSceneComponentRotation();
			break;
		}
		else return 0.f;
	}

	if (Type == 0)
	{
		if (!IgnoreRestrictions) RotatorValue.Yaw = UniversalCamera->GetClampedYaw(RotatorValue.Yaw);
		return RotatorValue.Yaw;
	}
	else if (Type == 1)
	{
		if (!IgnoreRestrictions) RotatorValue.Pitch = UniversalCamera->GetClampedYaw(RotatorValue.Pitch);
		return RotatorValue.Pitch;
	}
	else if (Type == 2)
	{
		if (!IgnoreRestrictions) RotatorValue.Roll = UniversalCamera->GetClampedYaw(RotatorValue.Roll);
		return RotatorValue.Roll;
	}

	return 0.f;
}

float FTargetFloat::GetZoomValue(FTargetSettings TargetSettings, bool& IsValid)
{
	IsValid = false;

	switch (TargetMod)
	{
	case TargetMod_CustomValue:
		IsValid = true;
		return FloatValue;
	case TargetMod_Actor:
		return TargetSettings.GetTargetActorZoom(IsValid);

	}

	return 0.f;
}

float FTargetFloat::GetDirection(AUniversalCamera* UniversalCamera, int32 Type, FTargetSettings TargetSettings, float StartingValue, bool IgnoreRestrictions, bool& IsValid)
{
	IsValid = false;

	if (Type < 3)
	{
		float RotationValue = GetRotationValue(UniversalCamera, Type, TargetSettings, IgnoreRestrictions, IsValid);
		float Delta = RotationValue - StartingValue;

		if (Delta > 180.0f)
		{
			Delta -= 360.0f;
		}
		else if (Delta < -180.0f)
		{
			Delta += 360.0f;
		}
		return Delta;
	}
	else if (Type == 3)
	{
		float ZoomValue = GetZoomValue(TargetSettings, IsValid);
		if (!IgnoreRestrictions) ZoomValue = UniversalCamera->GetClampedZoom(ZoomValue);
		return ZoomValue - StartingValue;
	}

	return 0.f;
}