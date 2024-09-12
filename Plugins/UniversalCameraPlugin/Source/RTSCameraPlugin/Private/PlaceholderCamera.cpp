// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceholderCamera.h"

// Sets default values
APlaceholderCamera::APlaceholderCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 400.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void APlaceholderCamera::BeginPlay()
{
	Super::BeginPlay();
}

FPlaceholderCameraInfos APlaceholderCamera::GetInfos()
{
	FPlaceholderCameraInfos Infos;
	Infos.Location = GetActorLocation();
	Infos.Yaw = GetActorRotation().Yaw;
	Infos.Pitch = GetActorRotation().Pitch;
	Infos.Zoom = GetZoom();

	return Infos;
}