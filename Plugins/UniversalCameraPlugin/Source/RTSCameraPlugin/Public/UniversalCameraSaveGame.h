// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SharedStructs.h"
#include "UniversalCameraSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSALCAMERAPLUGIN_API UUniversalCameraSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FUniversalCameraPositionSaveFormat SavedPosition;
	UPROPERTY()
	bool bIsValidSavePosition = false;

	UPROPERTY()
	TArray<uint8> SavedSettings;
	UPROPERTY()
	bool bIsValidSaveSettings = false;


};
