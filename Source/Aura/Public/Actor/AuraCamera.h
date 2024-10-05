// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UniversalCamera.h"
#include "AuraCamera.generated.h"

class UCameraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCamera : public AUniversalCamera
{
	GENERATED_BODY()

public:
	AAuraCamera();

	void SetCameraBoundaries();
	void RemoveCameraBoundaries();
	
	UCameraComponent* GetCameraComponent() const { return Camera; }

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

private:
	UFUNCTION()
	void OnLocationUpdate();
	UFUNCTION()
	void OnExitLocation();
};
