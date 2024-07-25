// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UINavigation/Public/UINavController.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"

#include "MainPlayerController.generated.h"

struct FTimeline;
enum class ETargetTeam : uint8;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerDeviceChanged, bool, bIsGamepad);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOcclusionChanged, bool, bShouldOcclude);

/**
 * 
 */
UCLASS()
class AURA_API AMainPlayerController : public AUINavController
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
	
	bool IsUsingGamepad() const { return bUsingGamepad; }
	UFUNCTION(BlueprintCallable)
	void ChangeUsingGamepad(bool bIsGamepad);
	FVector GetInputDirection() { return InputDirection; }
	bool IsTargeting() const { return bTargeting; }
	ATargetingActor* GetTargetingActor() const { return TargetingActor; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="ControllerDevice")
	FOnControllerDeviceChanged ControllerDeviceChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void ShowTargetingActor(
		TSubclassOf<ATargetingActor> TargetingActorClass,
		ETargetTeam TargetTeam,
		float Radius = 300.f);
	UFUNCTION(BlueprintCallable)
	void HideTargetingActor();

	void SetShouldOccludeObjects(bool bInShouldOcclude);

	void SetPlayerCamera(UCameraComponent* InCamera) { PlayerCamera = InCamera; }

	UFUNCTION(BlueprintImplementableEvent, Category="Camera|Occlusion")
	void OnOcclusionChange(bool bIsOccluding);

	UUINavPCComponent* GetUINavComponent() const { return UINavPCComp; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void HandleEnvironmentOcclusion();
	void GamepadMoveTargetingActor(const FInputActionValue& InputActionValue);

	UPROPERTY(BlueprintReadWrite, Category="Input")
	bool bUsingGamepad = false;

	UPROPERTY(BlueprintReadOnly, Category="Input")
	FVector InputDirection = FVector::Zero();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Occlusion")
	TObjectPtr<UMaterialParameterCollection> OcclusionMaskParameterCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera|Occlusion")
	float OcclusionSize = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Camera|Occlusion")
	bool bDebugCameraOcclusionTrace = false;

private:
	void Move(const FInputActionValue& InputActionValue);
	void MoveComplete(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	FHitResult CursorHit;
	AActor* LastActor = nullptr;
	AActor* ThisActor = nullptr;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void ConfirmPressed();
	void CancelPressed();

	void UpdateTargetingActorLocation();
	void UpdatePlayerLocationParameterCollection();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MainContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> TargetingActorMoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ConfirmAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CancelAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CheckInputSourceAction;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY()
	TObjectPtr<ATargetingActor> TargetingActor;
	
	bool bTargeting = false;

	// BEGIN Environment occlusion
	UPROPERTY()
	UMaterialParameterCollectionInstance* OcclusionMaskParameterCollectionInstance = nullptr;
	UMaterialParameterCollectionInstance* GetOcclusionMaskParameterCollectionInstance();
	bool bCameraOcclusionActive = false;
	FTimeline FadeTimeline = FTimeline();
	// END Environment occlusion

	UPROPERTY()
	UCameraComponent* PlayerCamera = nullptr;
};
