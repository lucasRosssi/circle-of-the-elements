// Fill out your copyright notice in the Description page of Project Settings.


#include "SharedStructs.h"
#include "GameFramework/SpringArmComponent.h"
#include "UniversalCamera.h"

bool FTargetSettings::IsValidActor() const
{
	return IsValid(Actor);
}

bool FTargetSettings::IsValidSocket() const
{
	return IsValid(Mesh) && Mesh->DoesSocketExist(Socket);
}

bool FTargetSettings::IsValidSceneComponent() const
{
	return IsValid(SceneComponent);
}

float FTargetSettings::GetTargetActorZoom(bool& IsValidTarget)
{
	IsValidTarget = false;
	if (!IsValidActor())
	{
		return 0.f;
	}

	//	Return the TargetArmLength if target has a SpringArmComponent
	USpringArmComponent* SpringArmComponent = Cast<USpringArmComponent>(Actor->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (IsValid(SpringArmComponent))
	{
		IsValidTarget = true;
		return SpringArmComponent->TargetArmLength;
	}
	//	Return the DesiredZoom is target is a UniversalCamera
	AUniversalCamera* UniversalCameraRef = Cast<AUniversalCamera>(Actor);
	if (IsValid(UniversalCameraRef))
	{
		IsValidTarget = true;
		return UniversalCameraRef->DesiredZoom;
	}

	return 0.f;
}

AActor* FTargetSettings::GetOwnerActor() const
{
	if (IsValidActor()) return Actor;
	if (IsValidSocket()) return Mesh->GetOwner();
	if (IsValidSceneComponent()) return SceneComponent->GetOwner();

	return nullptr;
}