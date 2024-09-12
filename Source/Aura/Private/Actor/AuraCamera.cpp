// Copyright Lucas Rossi


#include "Actor/AuraCamera.h"

#include "Camera/CameraComponent.h"
#include "Game/Components/LocationManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraCamera::AAuraCamera()
{
	MovementSpeed = 600.f;
	OffsetSpeed = 0.f;
	RotateSpeed = 0.f;
	ZoomSpeed = 0.f;
	IgnoreTimeDilation = true;
	ConstrainLocationToShape = true;
	bFollowActorIgnoreRestrictions = false;
	
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 4500.f;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetFieldOfView(30.f);
}

void AAuraCamera::SetCameraBoundaries()
{
	const TArray<AActor*> CameraBoundaries = UAuraSystemsLibrary::GetLocationManager(this)
		->GetCameraBoundaryActors();
	SetConstrainDots(GetActorsLocation2D(CameraBoundaries));
}

void AAuraCamera::RemoveCameraBoundaries()
{
	SetConstrainDots(TArray<FVector2d>());
}

void AAuraCamera::BeginPlay()
{
	Super::BeginPlay();

	ULocationManager* LocationManager = UAuraSystemsLibrary::GetLocationManager(this);
	if (LocationManager)
	{
		LocationManager->OnInitLocationDelegate.AddDynamic(this, &AAuraCamera::OnLocationUpdate);
		LocationManager->OnExitLocationDelegate.AddDynamic(this, &AAuraCamera::OnExitLocation);
	}
}

void AAuraCamera::OnLocationUpdate(EGatePosition GatePosition)
{
	SetCameraBoundaries();
}

void AAuraCamera::OnExitLocation(EGatePosition NextGatePosition)
{
	SetConstrainDots(TArray<FVector2d>());
}
