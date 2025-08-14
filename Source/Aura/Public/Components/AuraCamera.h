// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UniversalCamera.h"
#include "AuraCamera.generated.h"

class APostProcessVolume;
class USpotLightComponent;
enum class ECameraState : uint8;
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

  UFUNCTION(BlueprintCallable)
	void SetCameraBoundaries(const TArray<AActor*>& BoundaryActors);
	void SetCameraBoundaries();
	void RemoveCameraBoundaries();
	
	UCameraComponent* GetCameraComponent() const { return Camera; }
  APostProcessVolume* GetPostProcessVolume();

  void SetPlayerActor(AActor* InActor) { PlayerActor = InActor; }

  UFUNCTION(BlueprintCallable, Category="Camera|State")
  void SetCameraState(ECameraState State);

protected:

	virtual void BeginPlay() override;
  virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
  TObjectPtr<USpotLightComponent> SpotLight;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float DefaultFOV = 30.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float DefaultInterpSpeed = 5.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float DeathFOV = 15.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float DeathInterpSpeed = 10.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float SecondWindFOV = 25.f;
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
  float SecondWindInterpSpeed = 40.f;

  UPROPERTY(BlueprintReadWrite, Category="Camera|PostProcess")
  APostProcessVolume* PostProcessVolume;

private:
	UFUNCTION()
	void OnInitArea();
	UFUNCTION()
	void OnExitArea();

  void HandleDefaultState();
  void HandlePlayerDeathState();
  void HandleBossDeathState();
  void HandleSecondWindState();

  void InterpToDefaultFOV(float DeltaSeconds);
  void InterpToDeathFOV(float DeltaSeconds);
  void InterpToSecondWindFOV(float DeltaSeconds);

  ECameraState CameraState;

  TWeakObjectPtr<AActor> PlayerActor;
};
