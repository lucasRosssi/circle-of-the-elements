// Universal Camera Plugin - Mathieu Jacq 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "CollisionQueryParams.h"
#include "InputCoreTypes.h"
#include "SharedStructs.h"
#include "Kismet/KismetMathLibrary.h"
#include "UniversalCamera.generated.h"

UENUM(BlueprintType)
enum EEdgeScrollMode
{
	ScrollMovement				UMETA(DisplayName = "Movement"),
	ScrollTargetOffset			UMETA(DisplayName = "Target"),
	ScrollSocketOffset			UMETA(DisplayName = "Socket")

};

UENUM(BlueprintType)
enum ESimplifiedMovementDirection
{
	MovementDirection_None		UMETA(DisplayName = "None"),
	Direction_RightBackward		UMETA(DisplayName = "RightBackward"),
	Direction_Right				UMETA(DisplayName = "Right"),
	Direction_RightForward		UMETA(DisplayName = "RightForward"),
	Direction_Forward			UMETA(DisplayName = "Forward"),
	Direction_LeftForward		UMETA(DisplayName = "LeftForward"),
	Direction_Left				UMETA(DisplayName = "Left"),
	Direction_LeftBackward		UMETA(DisplayName = "LeftBackward"),
	Direction_Backward			UMETA(DisplayName = "Backward")
};

UENUM(BlueprintType)
enum EScreenSlidingType
{
	ScreenSliding_None			UMETA(DisplayName = "None"),
	SlideCamera					UMETA(DisplayName = "Camera"),
	SlideTargetOffset			UMETA(DisplayName = "TargetOffset"),
	SlideSocketOffset			UMETA(DisplayName = "SocketOffset"),
	SlideYaw					UMETA(DisplayName = "Yaw"),
	SlidePitch					UMETA(DisplayName = "Pitch"),
	SlideYawAndPitch			UMETA(DisplayName = "Yaw and Pitch")
};

UENUM(BlueprintType)
enum EConstrainType
{
	NoConstrain					UMETA(DisplayName = "None"),
	MaxDistance					UMETA(DisplayName = "MaxDistance"),
	CustomConstrain				UMETA(DisplayName = "Custom"),
};

UENUM(BlueprintType)
enum EStopFollowingReason
{
	StopFollowingReason_Manual				UMETA(DisplayName = "Manual"),
	StopFollowingReason_InvalidTarget		UMETA(DisplayName = "Invalid Target"),
	StopFollowingReason_DisabledFeature		UMETA(DisplayName = "Disabled Feature")
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FConstrainVector
{
	GENERATED_BODY()

	FConstrainVector() {}

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool X = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Y = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Z = false;
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FConstrainVector2
{
	GENERATED_BODY()

		FConstrainVector2() {}

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool X = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Y = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Z = true;

	bool IsValid() const { return (X || Y || Z); }
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FBoolRotation
{
	GENERATED_BODY()

		FBoolRotation() {}

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Yaw = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Pitch = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Restrictions")
		bool Roll = false;
};

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FTargetSettings_WithTemplates
{
	GENERATED_BODY()

	FTargetSettings TargetSettings;
	int32 Type = -1;

	FVector GetTargetLocation(bool& IsValid, FRotator& Rotation);

	FORCEINLINE void Empty()
	{
		FTargetSettings EmptySettings;
		TargetSettings = EmptySettings;
		Type = -1;
	}
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EEnabledFeatures
{
	None = 0 UMETA(Hidden),
//	Forward/Backward, Right/Left, Up/Down Movement.
	Movement = 1 << 0,
//	Yaw and Pitch Rotation.
	Rotation = 1 << 1,
//	Zoom.
	Zoom = 1 << 2,
//	Move the Camera when the mouse cursor comes close to the edges of the screen.
	EdgeScrolling = 1 << 3,
//	Move the camera by maintaining a key and moving the mouse around.
//	Activate/Deactivate it with "ToggleScreenSliding" when pressing/releasing a key (usually a mouse key).
	ScreenSliding = 1 << 4,
//	Make the Origin of the Universal Camera collide with the environment (see the "Origin Collisions" category in the defaults)
	OriginCollisions = 1 << 5		UMETA(DisplayName = "Origin Collisions (BETA)"),
//	Make the SpringArmComponent collide with the environment, so that you can always see the Origin
	SpringArmCollisions = 1 << 6	UMETA(DisplayName = "Spring Arm Collisions (BETA)"),
//	Correct the Z location of the camera to adapt to terrain.
//	Override "GetTerrainHeightAdaptationValue" if you prefer to implement your own adapatation method.
	TerrainHeightAdaptation = 1 << 7,
//	Lag applied to Forward/Backward, Right/Left, Up/Down Movement.
	LagMovement = 1 << 8,
//	Lag applied to Offset Movement.
	LagOffset = 1 << 9,
//	Lag applied to Yaw and Pitch Rotation.
	LagRotation = 1 << 10,
//	Lag applied to Zoom.
	LagZoom = 1 << 11,
//	Allows to follow an Actor with "FollowTarget" and "StopFollowing".
//	If you want to be able to move around while following an Actor, move the Offset of the camera while following it.
	FollowTarget = 1 << 12,
//	Use this to drag the Camera around with the mouse, while the cursor remains at the initial position.
//	Activate/Deactivate it with "TogglePreciseMouseDrag" when pressing/releasing a key (usually a mouse key).
	PreciseMouseDrag = 1 << 13,
};
ENUM_CLASS_FLAGS(EEnabledFeatures);

UCLASS()
class UNIVERSALCAMERAPLUGIN_API AUniversalCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUniversalCamera();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//	COMPONENTS

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UFUNCTION(BlueprintPure, Category = "Components")
	UCameraComponent* GetCameraComponent(bool& HasCameraAttached);

	//	INFO

	UPROPERTY(Replicated, Interp, BlueprintReadOnly, Category = "Universal Camera Plugin|Default")
		FVector DesiredLocation;
	UPROPERTY(Replicated, Interp, BlueprintReadOnly, Category = "Universal Camera Plugin|Default")
		FVector DesiredSocketOffset;
	UPROPERTY(Replicated, Interp, BlueprintReadOnly, Category = "Universal Camera Plugin|Default")
		FVector DesiredTargetOffset;
	UPROPERTY(Replicated, Interp, BlueprintReadOnly, Category = "Universal Camera Plugin|Default")
		FRotator DesiredRotation;
	UPROPERTY(Replicated, Interp, BlueprintReadOnly, Category = "Universal Camera Plugin|Default")
		FRotator DesiredRotationOffset = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(Replicated, Interp, BlueprintReadOnly, meta = (ClampMin = 0.0), Category = "Universal Camera Plugin|Default")
		float DesiredZoom;

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FORCEINLINE FRotator GetDesiredRotationWithOffset() const {return DesiredRotation + DesiredRotationOffset; }

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FORCEINLINE FVector GetTargetOffsetWorldLocation() const { return GetActorLocation() + SpringArmComponent->TargetOffset; }
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FORCEINLINE FVector GetDesiredTargetOffsetWorldLocation() const { return DesiredLocation + DesiredTargetOffset; }

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FORCEINLINE FVector GetSocketOffsetWorldLocation() const {
		return GetTargetOffsetWorldLocation() + FRotationMatrix(GetActorRotation()).TransformVector(SpringArmComponent->SocketOffset); }
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FORCEINLINE FVector GetDesiredSocketOffsetWorldLocation() const {
		return GetDesiredTargetOffsetWorldLocation() + FRotationMatrix(DesiredRotation).TransformVector(DesiredSocketOffset); }

	//	Returns the desired location of the CameraComponent
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Default")
	FVector GetDesiredCameraWorldLocation() const { return GetDesiredSocketOffsetWorldLocation() - DesiredRotation.Vector() * DesiredZoom; }
	FVector GetDesiredCameraWorldLocation(FVector DesiredSocketOffsetWorldLocation) const { return DesiredSocketOffsetWorldLocation - DesiredRotation.Vector() * DesiredZoom; }
	FVector GetDesiredCameraWorldLocation(FRotator Rotation) const { return GetDesiredSocketOffsetWorldLocation() - Rotation.Vector() * DesiredZoom; }
	FVector GetDesiredCameraWorldLocation(float Zoom) const { return GetDesiredSocketOffsetWorldLocation() - DesiredRotation.Vector() * Zoom; }

	FVector GetOriginLocationFromCameraLocation(FVector CameraLocation) const;

	//	Velocity
	FVector LastLocation;
	float LastDeltaTime = 1.f;
	FVector Velocity;

	FVector GetVelocity() const override { return GetActorLocation() - LastLocation; }

	//	TRAVEL TASK
	UFUNCTION()
	void StartTraveling_Internal(UAsyncNode* AsyncNode);
	UFUNCTION()
	void TravelTaskTick(float DeltaTime);
	void UpdateCameraTransform(float CurveValue);
	UFUNCTION()
	void EndTravelTask(const bool Abort = false);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|CameraTravel")
	void AbortTravelTask() { EndTravelTask(true); }

	UPROPERTY()
	class UAsyncNode* CurrentTravelTask;
	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|CameraTravel")
	bool IsTraveling = false;

	//	RESTRICTIONS

	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		FVector GetClampedLocation(FVector Location);
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		FVector GetClampedOffset(FVector Offset);
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		float GetClampedYaw(float Yaw);
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		float GetClampedPitch(float Pitch);
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		float GetClampedRoll(float Roll);
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
		float GetClampedZoom(float Zoom);

	//	Get the Corrected Destination from Restrictions
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Basic Movement & Zoom|Restrictions|Location")
		FVector GetCorrectedDestinationFromRestrictions(FVector Destination);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LOG_VerifyStartingSettings();

	//	TICK UPDATES
	void UpdateSpringArmLength(float DeltaTime);

	static FVector FindVectorAtZ(bool& Intersect, FVector Direction, float Z);

public:
		//	FEATURES
	
	//	Toggles the listed features On.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Features")
	void EnableFeatures(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/UniversalCameraPlugin.EEnabledFeatures")) int32 FeaturesToEnable);
	//	Toggles the listed features Off.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Features")
	void DisableFeatures(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/UniversalCameraPlugin.EEnabledFeatures")) int32 FeaturesToDisable);
	//	Returns true if all the tagged features are enabled.
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Features")
	bool AreFeaturesEnabled(UPARAM(meta = (Bitmask, BitmaskEnum = "/Script/UniversalCameraPlugin.EEnabledFeatures")) int32 FeaturesToTest);
protected:
	//	Features currently enabled
	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/UniversalCameraPlugin.EEnabledFeatures"), Category = "Universal Camera Plugin|Features")
	int32 EnabledFeatures = 16151;
public:
		//	TOOLS
	//	Use these to modify the camera position
	
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredPosition(FVector NewLocation, FVector NewTargetOffset, FVector NewSocketOffset, float NewYaw, float NewPitch, float NewRoll, float NewZoom, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredLocation(FVector NewLocation, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredTargetOffset(FVector NewTargetOffset, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredSocketOffset(FVector NewSocketOffset, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredYaw(float NewYaw, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredYawOffset(float NewYawOffset);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredPitch(float NewPitch, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredPitchOffset(float NewPitchOffset);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredRoll(float NewRoll, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredRollOffset(float NewRollOffset);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredRotation(FRotator NewRotation, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredRotationOffset(FRotator NewRotationOffset);
	//	@Param IgnoreRestrictions Ignore Restrictions and Collisions
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void SetDesiredZoom(float NewZoom, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	
protected:
	void SetDesiredLocation_Internal(FVector NewLocation, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredTargetOffset_Internal(FVector NewTargetOffset, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredSocketOffset_Internal(FVector NewSocketOffset, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredYaw_Internal(float NewYaw, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredPitch_Internal(float NewPitch, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredRoll_Internal(float NewPitch, bool IgnoreLag = false, bool IgnoreRestrictions = false);
	void SetDesiredZoom_Internal(float NewZoom, bool IgnoreLag = false, bool IgnoreRestrictions = false);

public:
	//	Returns the mouse position relative to the viewport size
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
	static FVector2D GetMousePositionRatio(APlayerController* PlayerController, FVector2D& RawPosition);
	//	Returns the ImpactPoint of the HitResult of "GetHitResultUnderCursor"
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Tools")
	static FVector GetImpactPointUnderCursor(APlayerController* PlayerController, ECollisionChannel TraceChannel, bool TraceComplex, bool& DidHit);

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		void RotateYawAroundPivot(FVector Pivot, float AxisValue, float Multiplier = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		void RotatePitchAroundPivot(FVector Pivot, float AxisValue, float Multiplier = 1.0f, bool LockZAxis= false);

	//	Resets the TargetOffset back to (0.0.0)
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		FORCEINLINE void ResetTargetOffset() { SetDesiredTargetOffset(FVector(0.f, 0.f, 0.f)); }
	//	Resets the SocketOffset back to (0.0.0)
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		FORCEINLINE void ResetSocketOffset() { SetDesiredSocketOffset(FVector(0.f, 0.f, 0.f)); }
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		FORCEINLINE void ResetRotationOffset() { SetDesiredRotationOffset(FRotator(0.f, 0.f, 0.f)); }
	// Returns the focus position for the Actor
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Universal Camera Plugin|Tools")
		bool GetActorFocusLocation(AActor* Actor, FVector& Location, float& IdealZoom);

	//	Zoom towards the location. Uses ZoomScaling settings.
	//	@param LockZoomLevel If false, move the Zoom Level and lock the Z axis of the Origin. If true, move the Origin instead of the Zoom Level. 
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
	void ZoomAtLocation(float AxisValue, FVector Location, float Multiplier = 1.0f, bool LockZoomLevel = false);

	//	Zoom towards the mouse location on the ground. Uses ZoomScaling settings.
	//	@param LockZoomLevel If false, move the Zoom Level and lock the Z axis of the Origin. If true, move the Origin instead of the Zoom Level. 
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Tools")
		void ZoomAtCursor(float AxisValue, ECollisionChannel CollisionChannel, bool& DidHit, float Multiplier = 1.0f, bool LockZoomLevel = false);

	//	Save & Load

	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Save")
	TArray<uint8> GetSettingsSaveFormat();
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
	void LoadSettingsSaveFormat(TArray<uint8> SettingsSaveFormat);

	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Save")
	FUniversalCameraPositionSaveFormat GetPositionSaveFormat();
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
	void LoadPositionSaveFormat(FUniversalCameraPositionSaveFormat PositionSaveFormat);

		// SaveGameObj
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
		void QuickSavePosition(FUniversalCameraPositionSaveFormat PositionSaveFormat, const FString SlotName, const int32 UserIndex);
	//	Will make sure the SaveGameObject has valid data (meaning the position has been saved before)
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
		void QuickLoadPosition(const FString SlotName, const int32 UserIndex);

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
		void QuickSaveSettings(TArray<uint8> SettingsSaveFormat, const FString SlotName, const int32 UserIndex);
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Save")
		void QuickLoadSettings(const FString SlotName, const int32 UserIndex);

protected:
	void InitUCSaveGameInstance();
	class UUniversalCameraSaveGame* UCSaveGameInstance;


	
	//	STARTING POSITION

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingLocation = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingLocation"), Category = "Universal Camera Plugin|Starting Position")
	FVector StartingLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingTargetOffset = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingTargetOffset"), Category = "Universal Camera Plugin|Starting Position")
	FVector StartingTargetOffset = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingSocketOffset = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingSocketOffset"), Category = "Universal Camera Plugin|Starting Position")
	FVector StartingSocketOffset = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingYaw = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingYaw"), Category = "Universal Camera Plugin|Starting Position")
	float StartingYaw = 0.0f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingPitch = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingPitch"), Category = "Universal Camera Plugin|Starting Position")
	float StartingPitch = -65.0f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Universal Camera Plugin|Starting Position")
	bool OverrideStartingZoom = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "OverrideStartingZoom"), Category = "Universal Camera Plugin|Starting Position")
	float StartingZoom = 500.0f;

public:
		
	//	BASIC MOVEMENT

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	// @param MoveTargetOffset - Set this to true to move the TargetOffset instead of the Origin.
	// @param MoveSocketOffset - Set this to true to move the SocketOffset instead of the Origin.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void MoveForward(float AxisValue, float Multiplier = 1.0f, bool MoveTargetOffset = false, bool MoveSocketOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	// @param MoveTargetOffset - Set this to true to move the TargetOffset instead of the Origin.
	// @param MoveSocketOffset - Set this to true to move the SocketOffset instead of the Origin.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void MoveRight(float AxisValue, float Multiplier = 1.0f, bool MoveTargetOffset = false, bool MoveSocketOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	// @param MoveTargetOffset - Set this to true to move the TargetOffset instead of the Origin.
	// @param MoveSocketOffset - Set this to true to move the SocketOffset instead of the Origin.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void MoveUp(float AxisValue, float Multiplier = 1.f, bool MoveTargetOffset = false, bool MoveSocketOffset = false);

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void Move(float ForwardAxis, float RightAxis, float UpAxis, float Multiplier = 1.f, bool NormalizeDirection = true, bool MoveTargetOffset = false, bool MoveSocketOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void RotateYaw(float AxisValue, float Multiplier = 1.f, bool RotateOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void RotatePitch(float AxisValue, float Multiplier = 1.f, bool RotateOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void RotateRoll(float AxisValue, float Multiplier = 1.f, bool RotateOffset = false);

	// @param AxisValue - Feed a negative value to go in the opposite direction.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin")
	void ZoomIn(float AxisValue, float Multiplier = 1.f);
	float GetZoomInValue(float AxisValue, float Multiplier = 1.f);
	//	Returns the ZoomIn value with a scaling dependent a distance instead of the current ZoomLevel (used for ZoomAtLocation)
	float GetZoomInValueFromDistance(float AxisValue, float Multiplier = 1.f, float Distance = 0.f);

	float GetScaledValue(float AxisValue, float Multiplier, float Speed, float ScalingValue, float Min, float Max) const;

		//	 BASIC MOVEMENT

	// MovementSpeed is used with MoveForward(), MoveRight(), ScreenSliding(), EdgeScrolling().
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom")
	float MovementSpeed = 10.0f;

	// If true, MoveForward() will ignore the Pitch to take the forward vector.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom")
	bool UseAlternativeForwardVector = true;

	// If true, MoveRight() will ignore the Roll to take the right vector.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom")
	bool UseAlternativeRightVector = true;

	// If true, MoveUp() will always move along the Z axis.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom")
	bool UseAlternativeUpVector = true;

	//	OffsetSpeed is used with MoveForward(), MoveRight(), ScreenSliding, EdgeScrolling when Offset is checked.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom")
		float OffsetSpeed = 10.0f;

	//	RotateSpeed is used with RotateYaw(), RotatePitch(), and ScreenSliding when Yaw or Pitch is used.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom")
		float RotateSpeed = 1.4f;

	//	ZoomSpeed is used with ZoomIn().
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom")
		float ZoomSpeed = 50.0f;

	//	Ignore the current TimeDilation value
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom")
	bool IgnoreTimeDilation;

protected:

	FORCEINLINE float GetTimeDilationCorrectiveValue() {return IgnoreTimeDilation ? 1.f / LastTimeDilation : 1.f;}
	FORCEINLINE float GetTickMultiplier() { return GetWorld()->GetDeltaSeconds() * 100.f * GetTimeDilationCorrectiveValue(); }
	float LastTimeDilation = 1.f;

public:

// Override this to make your own scaling method!
// You don't need to worry about MinimumMovementSpeed and MaximumMovementSpeed as it is automatically applied.
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin")
		float GetMovementScaling();

	// Adapt the Movement Speed depending on the distance from the ground.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	bool ApplyMovementScaling = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	bool ApplyOffsetScaling = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	bool ApplyMoveUpScaling = false;
	//	If true, the Movement Scaling will use the Camera Height instead of the Origin.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	bool UseZoomLevel = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.00001", UIMin = "0.00001"), Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	float MinimumMovementSpeed = 1.0f;
	//	Set it to 0 to ignore this setting.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	float MaximumMovementSpeed = 0.0f;
	//	Distance from the ground at which the camera will move at the default speed.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.00001", UIMin = "0.00001"), Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	float ScalingDistanceReference = 600.0f;
	//	If your ground level never changes in the level, set its Z location here.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	float GroundLevel = 0.0f;
	//	If true, will trace down the Z Axis to find the nearest ground and override GroundLevel.
	//	If you're working with great distances, consider increasing "TraceDistance" inside your PlayerController.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	bool ScalingTraceToGround = false;
	//	Channels to trace to the ground. 
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Movement Scaling")
	TArray<TEnumAsByte<ECollisionChannel>> ScalingTraceChannels;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Movement & Zoom|Zoom Scaling")
	bool UseZoomScaling = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.00001", UIMin = "0.00001", EditCondition = UseZoomScaling), Category = "Universal Camera Plugin|Movement & Zoom|Zoom Scaling")
	float MinimumZoomSpeed = 1.0f;
	//	Set it to 0 to ignore this setting
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = UseZoomScaling), Category = "Universal Camera Plugin|Movement & Zoom|Zoom Scaling")
	float MaximumZoomSpeed = 0.0f;
	//	Sets the scaling of your Zoom. The greater the distances you're working with are, the greater this value must be.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.00001", UIMin = "0.00001", EditCondition = UseZoomScaling), Category = "Universal Camera Plugin|Movement & Zoom|Zoom Scaling")
	float ZoomReference = 500.0f;

	// Override this to make your own scaling method!
	// You don't need to worry about MinimumZoomSpeed and MaximumZoomSpeed as it is automatically applied.
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin")
	float GetZoomInScaling();

	//	ORIGIN COLLISIONS

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Origin Collisions")
	bool ApplyOriginCollisions = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Origin Collisions")
	bool ApplyTargetOffsetCollisions = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Origin Collisions")
	bool ApplySocketOffsetCollisions = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Origin Collisions")
	TEnumAsByte<ECollisionChannel> OriginCollisionsChannel;
	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (ClampMin = 0.1), EditDefaultsOnly, Category = "Universal Camera Plugin|Origin Collisions")
	float OriginCollisionsProbeSize = 10.f;

	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin | Origin Collisions")
	FVector GetCorrectedDestinationFromOriginCollisions(FVector Location, FVector Destination, bool& DidHit);

	//	SPRING ARM COLLISIONS
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Spring Arm Collisions")
		TEnumAsByte<ECollisionChannel> SpringArmCollisionsChannel = ECC_Camera;

	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (ClampMin = 0.1), EditDefaultsOnly, Category = "Universal Camera Plugin|Spring Arm Collisions")
	float SpringArmCollisionsProbeSize = 10.f;

		//	TERRAIN HEIGHT ADAPTATION
		
	//	Override this function if you want to implement your own Z Correction depending on the environment for TerrainHeightAdaptation
	//	You can use the defaults in the section "Universal Camera Plugin | Terrain Height Adaptation" if you want
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin |Terrain Height Adaptation")
	float GetTerrainHeightAdaptationValue(bool& IsValid);

protected:

	void ApplyTerrainHeightAdaptationTick();

public:

	//	Collision Channel to test the probe against.
	//	Set the camera to react to this type of terrain
	// If you're working with great distances, consider increasing "TraceDistance" inside your PlayerController.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Terrain Height Adaptation")
	TArray<TEnumAsByte<ECollisionChannel>> TerrainHeightAdaptationChannels;
	//	Origin distance from the ground
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Terrain Height Adaptation")
	float OriginDistanceFromGround = 0.0f;
	//	Radius of the probe to see if there's valid ground above or under
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Terrain Height Adaptation")
	float ZProbeRadius = 50.0f;
	//	Enable to use camera height to test for upper ground (will allow you to go below surfaces instead of always looking for the highest ground)
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Terrain Height Adaptation")
	bool CanGoBelowSurfaces = true;
	//	If true, an additional check will make sure that you can always see Origin
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Terrain Height Adaptation")
	bool AlwaysSeeOrigin = false;
	//	Radius of the probe to check for Origin visibility
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = AlwaysSeeOrigin), Category = "Universal Camera Plugin|Terrain Height Adaptation")
	float VisibilityProbeRadius = 0;
	//	Allows the camera to ignore TerrainHeightAdaptation while at a certain height from the ground
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Terrain Height Adaptation")
	bool DistanceAdaptation = false;
	//	The height from which the camera will ignore TerrainHeightAdaptation
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = DistanceAdaptation), Category = "Universal Camera Plugin|Terrain Height Adaptation")
	float DistanceBypassValue = 1000;

		//	EDGE SCROLLING

	//	Override this to implement your own EdgeScrolling method
	//	
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin | Edge Scrolling")
	void EdgeScrollingTick(float RightAxis, float ForwardAxis, float RightMultiplier, float ForwardMultiplier, bool bTargetOffset, bool bSocketOffset);
	//	Will return the strenght multiplier of the EdgeScrolling. If "UseEdgeScrollingStrength" is false, will return 1.f.
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin | Edge Scrolling")
	void EdgeScrollingTick_Internal();

	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin | Edge Scrolling")
	void OnBeginEdgeScrolling();
	
	//	Used for UI events
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin | Edge Scrolling")
	void OnUpdateEdgeScrolling(ESimplifiedMovementDirection SimplifiedDirection, FVector2D MovementDirection);

	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin | Edge Scrolling")
	void OnEndEdgeScrolling();
	
protected:
	void OnBeginEdgeScrolling_Implementation() {}
	void OnUpdateEdgeScrolling_Implementation(ESimplifiedMovementDirection SimplifiedDirection, FVector2D MovementDirection) {}
	void OnEndEdgeScrolling_Implementation() {}
	
public:
	//	The distance from the edges of the screen at which your cursor must be to edge scroll
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.5", UIMax = "0.5"), Category = "Universal Camera Plugin|Edge Scrolling")
	float EdgeScrollingSensitivity = 0.2f;
	//	Speed of the edge scrolling movement
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Edge Scrolling")
	float EdgeScrollingSpeedMultiplier = 1.f;
	//	If enabled, the speed of the camera will adapt depending on the distance your cursor is from the border
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Edge Scrolling")
	bool UseEdgeScrollingStrength = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Edge Scrolling")
	bool NormalizeEdgeScrollingDirection = true;
	//	If enabled, move the SocketOffset instead of the Origin
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Edge Scrolling")
	TEnumAsByte<EEdgeScrollMode> EdgeScrollMode = ScrollMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|Edge Scrolling")
	bool IsEdgeScrolling = false;

		//	PRECISE MOUSE DRAG

	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Precise Mouse Control")
	void TogglePreciseMouseDrag(bool Activate);
protected:
	void PreciseMouseDragTick();

	public:
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta= (DisplayName="CollisionChannel"), Category = "Universal Camera Plugin|Precise Mouse Drag")
	TEnumAsByte<ECollisionChannel> PreciseMouseDragCollisionChannel;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta= (DisplayName="TraceComplex"), Category = "Universal Camera Plugin|Precise Mouse Drag")
	bool PreciseMouseDragTraceComplex;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "IgnoreMovementLag"), Category = "Universal Camera Plugin|Precise Mouse Drag")
	bool PreciseMouseDragIgnoreMovementLag = false;

	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|Precise Mouse Drag")
	bool IsPreciseMouseDragging = false;
	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|Precise Mouse Drag")
	FVector PreciseMouseDragOrigin;


		//	SCREEN SLIDING
	
	//	When you activate screen sliding, the position of your cursor will be stored.
	//	The camera will then move depending of the position of your cursor compared to its initial position
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Screen Sliding")
	void ToggleScreenSliding(bool Activate);
protected:
	void ScreenSlidingTick();
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Universal Camera Plugin|Screen Sliding")
	void OnBeginScreenSliding(FVector2D OriginPosition);
	UFUNCTION(BlueprintImplementableEvent, Category = "Universal Camera Plugin|Screen Sliding")
	void OnUpdateScreenSliding(FVector2D Direction);
	UFUNCTION(BlueprintImplementableEvent, Category = "Universal Camera Plugin|Screen Sliding")
	void OnEndScreenSliding();

	//	The Maximum Speed of the Screen Sliding, reached whenever your cursor distance from its initial position is more than the ScreenSlidingMaxRatio.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Screen Sliding")
	float ScreenSlidingMaxSpeed = 1.0f;
	// Max effective distance from initial cursor position
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (ClampMin = "0.0", UIMin = "0.0"), Category = "Universal Camera Plugin|Screen Sliding")
	float ScreenSlidingMaxRatio = 0.2f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Screen Sliding")
	bool ScreenSlidingInvertXAxis = false;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Screen Sliding")
	bool ScreenSlidingInvertYAxis = false;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Screen Sliding")
	TEnumAsByte<EScreenSlidingType> ScreenSlidingType = SlideCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|Screen Sliding")
	bool IsScreenSliding = false;

	protected:
	FVector2D ScreenSlidingOriginRatio;

	public:
		//	FOLLOW TARGET
	
	//	Follow the target Actor (Mesh and Socket are optional)
	//	You won't be able to move the Origin while following, however you can move the Offset
	//	@Param ResetOffsetIfPrematurelyEnded Will reset the offset if the task prematurely ended (invalid target, disabled feature, etc.)
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Follow Actor")
	void FollowTarget(FTargetSettings_WithTemplates TargetSettings, FConstrainVector2 FollowAxis, FBoolRotation FollowRotations, bool ResetOffsetIfPrematurelyEnded = false);

	//	Stops following the current Actor
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Follow Actor")
		FORCEINLINE void StopFollowing(bool bResetOffset = false) { StopFollowing_Internal(bResetOffset, StopFollowingReason_Manual); }

	UFUNCTION(BlueprintImplementableEvent, Category = "Universal Camera Plugin|Follow Actor")
	void OnStoppedFollowing(EStopFollowingReason Reason);
	
protected:
	UFUNCTION()
	void FollowTargetTick();
	void StopFollowing_Internal(bool bResetOffset, EStopFollowingReason Reason);
	void OnStoppedFollowing_Implementation(EStopFollowingReason Reason);


public:
	//	Returns true if the Camera is currently following an Actor
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Follow Actor")
		bool IsFollowingAnyActor() { return bIsFollowingAnyActor; }
	
	UPROPERTY(BlueprintReadOnly, Category = "Universal Camera Plugin|Follow Actor")
	FTargetSettings_WithTemplates FollowTargetSettings;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Follow Actor")
	bool bFollowActorIgnoreRestrictions = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Follow Actor")
	FConstrainVector2 FollowedAxis;
	FBoolRotation FollowedRotations;

protected:

	UPROPERTY()
	bool bIsFollowingAnyActor = false;
	UPROPERTY()
	bool bShouldResetOffsetIfFailed = false;

public:
		//	LAG

	//	Movement

	//	On each Tick, this function will gradually make the Origin location closer to its DesiredLocation
	//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	FVector Lag_GetTickDesiredLocation(float DeltaTime);

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float MovementLagSpeed = 5.0f;
	

	//	TargetOffset

//	On each Tick, this function will gradually make the TargetOffset closer to its DesiredTargetOffset location
//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Universal Camera Plugin|Movement & Zoom|Lag")
		FVector Lag_GetTickDesiredTargetOffset(float DeltaTime);

	//	SocketOffset

//	On each Tick, this function will gradually make the SocketOffset closer to its DesiredSocketOffset location
//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Universal Camera Plugin|Movement & Zoom|Lag")
		FVector Lag_GetTickDesiredSocketOffset(float DeltaTime);

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = UseOffsetLag), Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float OffsetLagSpeed = 5.0f;

		//	Height

	//	On each Tick, this function will gradually make the Origin Height closer to its desired Height (Same as for "Lag_GetTickDesiredOffset" but for the Z axis)
	//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float Lag_GetTickDesiredHeight(float DeltaTime);

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float HeightLagSpeed = 5.0f;

		//	Rotation

	//	On each Tick, this function will gradually make the Origin Rotation closer to its DesiredRotation
	//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	FRotator Lag_GetTickDesiredRotation(float DeltaTime);

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float RotationLagSpeed = 5.0f;

		//	Zoom

	//	On each Tick, this function will gradually make the Zoom closer to its DesiredZoom
	//	Override this function if you want to implement your own method!
	UFUNCTION(BlueprintNativeEvent, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float Lag_GetTickDesiredZoom(float DeltaTime);

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Lag")
	float ZoomLagSpeed = 5.0f;

		//	RESTRICTIONS
		
	//	Location
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Raw")
	FConstrainVector ConstrainLocationMinimum;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Raw")
	FVector MinimumLocation;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Raw")
	FConstrainVector ConstrainLocationMaximum;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Raw")
	FVector MaximumLocation;

	

	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Dots Shape")
	static TArray<FVector2D> GetActorsLocation2D(TArray<AActor*> Actors);

protected:
	FVector GetCorrectedDestinationFromDotsShape(const FVector& Destination, const TArray<FVector2D>& PolygonPoints);
	bool IsPointInPolygon(const FVector2D& Point, const TArray<FVector2D>& PolygonPoints);
	FVector2D GetClosestPointOnPolygon(const FVector2D& Point, const TArray<FVector2D>& PolygonPoints);

public:
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Dots Shape")
	void ToggleConstrainDots(bool ConstrainWithDots) { ConstrainLocationToShape = ConstrainWithDots; }
	UFUNCTION(BlueprintCallable, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Dots Shape")
	void SetConstrainDots(TArray<FVector2D> Dots) { ConstrainDots = Dots; }

	UPROPERTY(SaveGame, BlueprintReadOnly, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Dots Shape")
	bool ConstrainLocationToShape = false;
	//	The dots are used to draw a 2D shape. The last position of the array will close the shape going back to the first element.
	//	If "ConstrainLocationToShape" is enabled, the Origin will only be able to move within that shape.
	//	You need at least three dots to draw a shape.
	UPROPERTY(SaveGame, BlueprintReadOnly, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Location|Dots Shape")
	TArray<FVector2D> ConstrainDots;
	
	//	Offset
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	TEnumAsByte<EConstrainType> OffsetConstrainType;
	//	Represents the max distance the Offset can be from the Origin.
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	float OffsetMaxDistance = 1000.f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	FConstrainVector ConstrainOffsetMinimum;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	FVector MinimumOffset;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	FConstrainVector ConstrainOffsetMaximum;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Offset")
	FVector MaximumOffset;
	
	//	Yaw

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Yaw")
	bool ConstrainYaw;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainYaw), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Yaw")
	float MinimumYaw;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainYaw), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Yaw")
	float MaximumYaw;
	//	Useful if you rotation is around -180 / 180
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainYaw), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Yaw")
	bool ReverseYawRestriction = false;
	  
	//	Pitch

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Pitch")
	bool ConstrainPitch = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Pitch")
	float MinimumPitch = -89.9f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Pitch")
	float MaximumPitch = -10.0f;
	//	Useful if you rotation is around -180 / 180
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Pitch")
	bool ReversePitchRestriction = false;

	//	Roll

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Roll")
	bool ConstrainRoll = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Roll")
	float MinimumRoll = -180.f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Roll")
	float MaximumRoll = 180.f;
	//	Useful if you rotation is around -180 / 180
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainPitch), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Roll")
	bool ReverseRollRestriction = false;

	//	Zoom

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Zoom")
	bool ConstrainZoomMinimum = true;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainZoomMinimum), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Zoom")
	float MinimumZoom = 0.0f;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Zoom")
	bool ConstrainZoomMaximum;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, meta = (EditCondition = ConstrainZoomMaximum), Category = "Universal Camera Plugin|Movement & Zoom|Restrictions|Zoom")
	float MaximumZoom;

		//	DEBUG

	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Debug")
	bool DrawDebugSpheres = true;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditDefaultsOnly, Category = "Universal Camera Plugin|Debug")
	bool DebugPrintDesiredPosition = false;

protected:
	//	Green:	Current Location
	//	Yellow: Desired Location
	// 	Orange:	Desired Target Offset
	//	Red:	Desired Socket Offset
	void PrintDesiredPosition(const float Duration = 0.f);

	
	static FVector GetLinePlaneIntersection(FVector Direction, FVector Origin, FVector PlaneNormal, FVector PlaneCoord, bool& Intesect);

public:
	//	CHARACTER
	//	Tools to help Character Implementation with the Camera

	//	Returns the Forward Vector using only the Yaw axis
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Character Tools")
	static FVector GetAlternativeForwardVector(FRotator Rotation);
	//	Returns the Right Vector using only the Yaw axis
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Character Tools")
	static FVector GetAlternativeRightVector(FRotator Rotation);
	//	Returns the Up Vector moving only on the Z axis
	UFUNCTION(BlueprintPure, Category = "Universal Camera Plugin|Character Tools")
	static FVector GetAlternativeUpVector(FRotator Rotation);

		//	TEMPLATES

	//	FTargetSettings_WithTemplates

	UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "FollowTarget|Settings")
		static FTargetSettings_WithTemplates UseActor(AActor* Actor)
	{
		FTargetSettings_WithTemplates Settings;
		Settings.TargetSettings.Actor = Actor;
		Settings.Type = 0;
		return Settings;
	}

	UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "FollowTarget|Settings")
		static FTargetSettings_WithTemplates UseSceneComponent(USceneComponent* SceneComponent)
	{
		FTargetSettings_WithTemplates Settings;
		Settings.TargetSettings.SceneComponent = SceneComponent;
		Settings.Type = 1;
		return Settings;
	}

	UFUNCTION(BlueprintPure, meta = (Keywords = "Make"), Category = "FollowTarget|Settings")
		static FTargetSettings_WithTemplates UseSocket(UMeshComponent* MeshComponent, FName Socket)
	{
		FTargetSettings_WithTemplates Settings;
		Settings.TargetSettings.Mesh = MeshComponent;
		Settings.TargetSettings.Socket = Socket;
		Settings.Type = 2;
		return Settings;
	}

protected:
		//	DEV

	void ClampCustomValue(bool ConstrainMinimum, bool ConstrainMaximum, float* ValueToClamp, float Minimum, float Maximum, bool ReverseRestrictions = false)
	{
		if (!(ConstrainMinimum || ConstrainMaximum)) return;

		if (ReverseRestrictions)
		{
			if ((*ValueToClamp < Maximum) && (*ValueToClamp > Minimum))
			{
				float ClampedValue = FMath::Abs(*ValueToClamp - Maximum) > FMath::Abs(*ValueToClamp - Minimum) ? Minimum : Maximum;
				*ValueToClamp = ClampedValue;
			}
		}
		else
		{
			*ValueToClamp = FMath::Clamp(*ValueToClamp, ConstrainMinimum ? Minimum : *ValueToClamp, ConstrainMaximum ? Maximum : *ValueToClamp);
		}
	}
};
