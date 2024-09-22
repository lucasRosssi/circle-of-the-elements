// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SharedStructs.h"
#include "AsyncNode.generated.h"


class AUniversalCamera;
class UCurveFloat;

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FTravelSpeedSettings
{
	GENERATED_BODY()

	FTravelSpeedSettings() {}

	UPROPERTY()
	bool UseSpeed = false;
	UPROPERTY()
		float Speed = 1000.f;
	UPROPERTY()
		float Duration = 2.f;

	void DurationInitializer(bool IsValidBool, float Direction, float* DurationRef, float Multiplier);
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FTargetVector
{
	GENERATED_BODY()

		FTargetVector() {}


	UPROPERTY()
		TEnumAsByte<ETargetMod> TargetMod = ETargetMod::TargetMod_None;
	UPROPERTY()
		class USplineComponent* SplineComponent = nullptr;
	UPROPERTY()
		FVector VectorValue = FVector(0.f, 0.f, 0.f);

	FVector GetLocation(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, bool& IsValid);
	FVector GetDirection(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, FVector StartingLocation, bool IgnoreRestrictions, bool& IsValid);
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FOffsetSettings
{
	GENERATED_BODY()

		FOffsetSettings() {}

	//	0 - None
	//	1 - Target Offset
	//	2 - Socket Offset
	UPROPERTY()
	int32 OffsetMod = 0;
	UPROPERTY()
	FVector OffsetValue = FVector(0.f, 0.f, 0.f);

	FVector GetDirection(AUniversalCamera* UniversalCamera, bool IgnoreRestrictions, bool& IsValidOffset);
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FTargetFloat
{
	GENERATED_BODY()

		FTargetFloat() {}

	UPROPERTY()
		TEnumAsByte<ETargetMod> TargetMod = ETargetMod::TargetMod_None;
	UPROPERTY()
		float FloatValue = 0.f;

	//	0 for Yaw, 1 for Pitch, 2 for Roll, 3 for Zoom
	float GetDirection(AUniversalCamera* UniversalCamera, int32 Type, FTargetSettings TargetSettings, float StartingValue, bool IgnoreRestrictions, bool& IsValid);

	float GetRotationValue(AUniversalCamera* UniversalCamera, int32 Type, FTargetSettings TargetSettings, bool IgnoreRestrictions, bool& IsValid);
	float GetZoomValue(FTargetSettings TargetSettings, bool& IsValid);
};

UCLASS()
class UNIVERSALCAMERAPLUGIN_API UAsyncNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//	CameraTravel to the specified Location.
	//	Drag any pin and type in "Use" with Context Sensitive enabled to see the different options.
	//	Use the "AbortTravelTask()" function to abort.
	// 	All of the settings are optional, but at least one must be valid.
	//	@Param TargetSettings Set the Target References that can be used by other settings.
	//	@Param Curve Must go from (0;0) to (1;1).
	//	@Param LockAllMovement If true, Movement, Rotation and Zoom will be locked during the traveling. If false, only valid settings will be locked.
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions 
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncNode* CameraTravel(AUniversalCamera* UniversalCamera, FTargetSettings TargetSettings, FTargetVector LocationSettings, FOffsetSettings OffsetSettings, FTargetFloat YawSettings, FTargetFloat PitchSettings, FTargetFloat RollSettings, FTargetFloat ZoomSettings, UCurveFloat* Curve, FTravelSpeedSettings SpeedSettings, bool LockAllMovement, bool IgnoreLag, bool IgnoreRestrictions);

	//	Camera
	AUniversalCamera* m_UniversalCamera;
	bool m_IgnoreRestrictions = false;
	bool m_IgnoreLag = false;

	//	Time
	float MaxDuration = 0.f;
	float ElapsedTime = 0.f;
	bool IgnoreTimeDilation = false;
	//	Duration
	FTargetSettings TargetSettings;
	//	Location
	FTargetVector LocationSettings;
	FVector StartingLocation;
	FVector LocationDirection;
	bool IsValidLocation;
	//	Offset
	FOffsetSettings OffsetSettings;
	FVector StartingOffset;
	FVector OffsetDirection;
	bool IsValidOffset;
	//	Yaw
	FTargetFloat YawSettings;
	float StartingYaw;
	float YawDirection;
	bool IsValidYaw;
	//	Pitch
	FTargetFloat PitchSettings;
	float StartingPitch;
	float PitchDirection;
	bool IsValidPitch;
	//	Roll
	FTargetFloat RollSettings;
	float StartingRoll;
	float RollDirection;
	bool IsValidRoll;
	//	Zoom
	FTargetFloat ZoomSettings;
	float StartingZoom;
	float ZoomDirection;
	bool IsValidZoom;
	//	Curve
	UCurveFloat* m_Curve;
	//	Lock Movement
	bool m_LockAllMovement = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskResult);
		
	//	Called if the task ended and the camera successfully reached the destination
	UPROPERTY(BlueprintAssignable)
	FOnTaskResult OnReachedDestination;
	//	Called if the task ended prematurely
	UPROPERTY(BlueprintAssignable)
	FOnTaskResult OnAborted;


	FORCEINLINE bool IsValidTargetLocation(AUniversalCamera* UniversalCamera) { bool IsValid; LocationSettings.GetLocation(m_UniversalCamera, TargetSettings, IsValid); return IsValid; }
	FORCEINLINE FVector GetLocationDirection(bool& IsValid) { return LocationSettings.GetDirection(m_UniversalCamera, TargetSettings, StartingLocation, m_IgnoreRestrictions, IsValid); }

	//	GetDirection: 0 Yaw - 1 Pitch - 2 Roll - 3 Zoom

	FORCEINLINE float IsValidTargetYaw() { bool IsValid; YawSettings.GetRotationValue(m_UniversalCamera, 0, TargetSettings, m_IgnoreRestrictions, IsValid); return IsValid; }
	FORCEINLINE float GetYawDirection(bool& IsValid) { return YawSettings.GetDirection(m_UniversalCamera, 0, TargetSettings, StartingYaw, m_IgnoreRestrictions, IsValid); }

	FORCEINLINE float IsValidTargetPitch() { bool IsValid; PitchSettings.GetRotationValue(m_UniversalCamera, 1, TargetSettings, m_IgnoreRestrictions, IsValid); return IsValid; }
	FORCEINLINE float GetPitchDirection(bool& IsValid) { return PitchSettings.GetDirection(m_UniversalCamera, 1, TargetSettings, StartingPitch, m_IgnoreRestrictions, IsValid); }

	FORCEINLINE float IsValidTargetRoll() { bool IsValid; RollSettings.GetRotationValue(m_UniversalCamera, 2, TargetSettings, m_IgnoreRestrictions, IsValid); return IsValid; }
	FORCEINLINE float GetRollDirection(bool& IsValid) { return RollSettings.GetDirection(m_UniversalCamera, 2, TargetSettings, StartingRoll, m_IgnoreRestrictions, IsValid); }

	FORCEINLINE float IsValidTargetZoom() { bool IsValid; ZoomSettings.GetZoomValue(TargetSettings, IsValid); return IsValid; }
	FORCEINLINE float GetZoomDirection(bool& IsValid) { return ZoomSettings.GetDirection(m_UniversalCamera, 3, TargetSettings, StartingZoom, m_IgnoreRestrictions, IsValid); }

	void TryUpdateLocationDirectionAndValidity();
	FORCEINLINE void TryUpdateYawDirectionAndValidity()		{ TryUpdateFloatDirectionAndValidity(&YawSettings, &YawDirection, &UAsyncNode::GetYawDirection, &IsValidYaw); }
	FORCEINLINE void TryUpdatePitchDirectionAndValidity()	{ TryUpdateFloatDirectionAndValidity(&PitchSettings, &PitchDirection, &UAsyncNode::GetPitchDirection, &IsValidPitch); }
	FORCEINLINE void TryUpdateRollDirectionAndValidity()	{ TryUpdateFloatDirectionAndValidity(&RollSettings, &RollDirection, &UAsyncNode::GetRollDirection, &IsValidRoll); }
	FORCEINLINE void TryUpdateZoomDirectionAndValidity()	{ TryUpdateFloatDirectionAndValidity(&ZoomSettings, &ZoomDirection, &UAsyncNode::GetZoomDirection, &IsValidZoom); }

	FORCEINLINE bool AreValidSettings() const { return (IsValidLocation || IsValidOffset || IsValidYaw || IsValidPitch || IsValidRoll || IsValidZoom); }

protected:

	typedef float (UAsyncNode::*DirectionFunction)(bool&);
	void TryUpdateFloatDirectionAndValidity(FTargetFloat* FloatSettings, float* FloatValue, DirectionFunction GetFloatDirection, bool* IsValidBool);

	 //	MAKE BP SETTINGS (USER FRIENDLY)



	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		static FTargetVector UseCustomLocation(const FVector Location)
	 {
		 FTargetVector Settings;
		 Settings.TargetMod = TargetMod_CustomValue;
		 Settings.VectorValue = Location;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetVector UseSpline(class USplineComponent* SplineComponent)
	 {
		 FTargetVector Settings;
		 Settings.TargetMod = TargetMod_Spline;
		 Settings.SplineComponent = SplineComponent;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetVector UseActorLocation()
	 {
		 FTargetVector Settings;
		 Settings.TargetMod = TargetMod_Actor;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetVector UseSocketLocation()
	 {
		 FTargetVector Settings;
		 Settings.TargetMod = TargetMod_Socket;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetVector UseSceneComponentLocation()
	 {
		 FTargetVector Settings;
		 Settings.TargetMod = TargetMod_SceneComponent;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetFloat UseCustomValue(const float Value = 0.f)
	 {
		 FTargetFloat Settings;
		 Settings.TargetMod = TargetMod_CustomValue;
		 Settings.FloatValue = Value;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetFloat UseActorValue()
	 {
		 FTargetFloat Settings;
		 Settings.TargetMod = TargetMod_Actor;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetFloat UseSocketValue()
	 {
		 FTargetFloat Settings;
		 Settings.TargetMod = TargetMod_Socket;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTargetFloat UseSceneComponentValue()
	 {
		 FTargetFloat Settings;
		 Settings.TargetMod = TargetMod_SceneComponent;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTravelSpeedSettings UseSpeed(const float Speed = 1000.f)
	 {
		 FTravelSpeedSettings Settings;
		 Settings.UseSpeed = true;
		 Settings.Speed = Speed;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FTravelSpeedSettings UseDuration(const float Duration = 2.f)
	 {
		 FTravelSpeedSettings Settings;
		 Settings.UseSpeed = false;
		 Settings.Duration = Duration;
		 return Settings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FOffsetSettings UseTargetOffset(const FVector TargetOffset)
	 {
		 FOffsetSettings OffsetSettings;
		 OffsetSettings.OffsetMod = 1;
		 OffsetSettings.OffsetValue = TargetOffset;
		 return OffsetSettings;
	 }

	 UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "AsyncNode|Settings")
		 static FOffsetSettings UseSocketOffset(const FVector SocketOffset)
	 {
		 FOffsetSettings OffsetSettings;
		 OffsetSettings.OffsetMod = 2;
		 OffsetSettings.OffsetValue = SocketOffset;
		 return OffsetSettings;
	 }

//	virtual void Activate() override;
};
