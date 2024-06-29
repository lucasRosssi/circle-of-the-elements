// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"

#include "MainPlayerController.generated.h"

class ATargetingActor;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ITargetInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccluded = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerDeviceChanged, bool, bIsGamepad);

/**
 * 
 */
UCLASS()
class AURA_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(
		float DamageAmount,
		ACharacter* TargetCharacter,
		bool bParried,
		bool bCriticalHit,
		bool bIsPlayer = false
	);
	
	bool GetUsingGamepad() { return bUsingGamepad; }
	UFUNCTION(BlueprintCallable)
	void ChangeUsingGamepad(bool bIsGamepad);
	FVector GetInputDirection() { return InputDirection; }

	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();

	UPROPERTY(BlueprintAssignable, Category="ControllerDevice")
	FOnControllerDeviceChanged ControllerDeviceChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void ShowTargetingActor(TSubclassOf<ATargetingActor> TargetingActorClass);
	UFUNCTION(BlueprintCallable)
	void HideTargetingActor();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bUsingGamepad = false;

	UPROPERTY(BlueprintReadOnly, Category="Input")
	FVector InputDirection = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion", meta=
	(ClampMin="0", ClampMax="600.0"))
	float DistanceFromTraceEndToCharacter = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion", meta=
	(ClampMin="0.1", ClampMax="10.0"))
	float CapsulePercentageForTrace = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	TObjectPtr<UMaterialInterface> FadeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
	bool bIsOcclusionEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool bDebugLineTraces = false;

	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	USpringArmComponent* ActiveSpringArm;
 
	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	UCameraComponent* ActiveCamera;
 
	UPROPERTY(BlueprintReadWrite, Category="Camera Occlusion|Components")
	UCapsuleComponent* ActiveCapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MainContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CheckInputSourceAction;
	
	void Move(const FInputActionValue& InputActionValue);
	void MoveComplete(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	FHitResult CursorHit;
	ITargetInterface* LastActor = nullptr;
	ITargetInterface* ThisActor = nullptr;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY()
	TObjectPtr<ATargetingActor> TargetingActor;

	void UpdateTargetingActorLocation();

	// Environment occlusion
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;
	bool HideOccludedActor(const AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActors();
	FORCEINLINE bool ShouldCheckCameraOcclusion() const
	{
		return bIsOcclusionEnabled && FadeMaterial && ActiveCamera && ActiveCapsuleComponent;
	}
	// END Environment occlusion
};
