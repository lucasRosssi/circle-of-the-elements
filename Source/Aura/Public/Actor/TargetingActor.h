// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingActor.generated.h"

class UFloatingPawnMovement;
enum class ETargetTeam : uint8;
class USphereComponent;

UCLASS()
class AURA_API ATargetingActor : public APawn
{
	GENERATED_BODY()
	
public:	
	ATargetingActor();

	virtual void Tick(float DeltaTime) override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce = false) override;

	void SetSourceActor(AActor* InActor) { SourceActor = InActor; }
	void SetTargetTeam(ETargetTeam InTargetTeam) { TargetTeam = InTargetTeam; }
	void SetTargetingRadius(float Radius);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> TargetingSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> TargetingDecal;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

private:
	UFUNCTION()
	void OnTargetingBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);
	UFUNCTION()
	void OnTargetingEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY()
	AActor* SourceActor;

	ETargetTeam TargetTeam;
};
