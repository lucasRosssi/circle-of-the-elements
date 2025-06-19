// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitManagerComponent.generated.h"


class ASpiritActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UOrbitManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrbitManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  
  void RegisterSpirit(ASpiritActor* Spirit);
  void UnregisterSpirit(ASpiritActor* Spirit);
protected:
	virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MinInterpSpeed = 1.f;
  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MaxInterpSpeed = 20.f;

  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MinOrbitRadius = 50.f;
  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MaxOrbitRadius = 150.f;

  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MinOrbitHeightOffset = 50.f;
  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MaxOrbitHeightOffset = 100.f;

  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MinOrbitSpeed = -60.f;
  UPROPERTY(EditAnywhere, Category = "Orbit")
  float MaxOrbitSpeed = 60.f;

private:
  UPROPERTY()
  TArray<ASpiritActor*> ActiveSpirits;

  TArray<float> CurrentOrbitAngle;
  
  TArray<float> OrbitSpeed;
  TArray<float> OrbitRadius;
  TArray<float> OrbitHeightOffset;
  TArray<float> InterpSpeed;

};
