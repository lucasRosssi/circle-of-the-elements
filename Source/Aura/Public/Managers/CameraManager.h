// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CameraManager.generated.h"

class AAuraCamera;
/**
 * 
 */
UCLASS()
class AURA_API ACameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
  ACameraManager();

  UFUNCTION(BlueprintPure, Category="Camera", meta=(DefaultToSelf="Target", HidePin="Target"))
  AAuraCamera* GetActiveCamera();

private:
  UPROPERTY()
  TObjectPtr<AAuraCamera> ActiveCamera;
};
