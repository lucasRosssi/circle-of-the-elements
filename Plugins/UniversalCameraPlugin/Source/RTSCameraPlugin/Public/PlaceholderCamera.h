// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlaceholderCamera.generated.h"

USTRUCT(Blueprintable)
struct UNIVERSALCAMERAPLUGIN_API FPlaceholderCameraInfos
{
	GENERATED_BODY()

		FPlaceholderCameraInfos() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlaceholderCameraInfos")
		FVector Location = FVector(0.f, 0.f, 0.f);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlaceholderCameraInfos")
		float Yaw = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlaceholderCameraInfos")
		float Pitch = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlaceholderCameraInfos")
		float Zoom = 0.f;
};

UCLASS()
class UNIVERSALCAMERAPLUGIN_API APlaceholderCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceholderCamera();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
		TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UFUNCTION(BlueprintPure, Category = "PlaceholderCameraInfos")
	float GetZoom() { return SpringArmComponent->TargetArmLength; }

	UFUNCTION(BlueprintPure, Category = "PlaceholderCameraInfos")
	FPlaceholderCameraInfos GetInfos();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
