// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "UINavigation/Public/UINavController.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/PlayerInterface.h"

#include "AuraPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UHealTextComponent;
class UInteractComponent;
class AAuraHUD;
class UUIManager;
struct FTimeline;
enum class ETargetTeam : uint8;
class ATargetingActor;
class UCapsuleComponent;
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
class AURA_API AAuraPlayerController : public AUINavController
{
  GENERATED_BODY()

public:
  AAuraPlayerController();
  virtual void PlayerTick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  void EnableController();
  UFUNCTION(BlueprintCallable)
  void DisableController();

  UFUNCTION(Client, Reliable)
  void ShowDamageNumber(
    float DamageAmount,
    ACharacter* TargetCharacter,
    bool bParried,
    bool bCriticalHit,
    bool bIsPlayer = false
  );
  UFUNCTION(Client, Reliable)
  void ShowHealNumber(
    float HealAmount,
    ACharacter* TargetCharacter,
    bool bIsPlayer = false
  );

  bool IsUsingGamepad() const { return bUsingGamepad; }
  UFUNCTION(BlueprintCallable)
  void ChangeUsingGamepad(bool bIsGamepad);
  FVector GetInputDirection() { return InputDirection; }
  bool IsTargeting() const { return bTargeting; }
  ATargetingActor* GetTargetingActor() const { return TargetingActor; }

  UFUNCTION(BlueprintCallable)
  void ShowTargetingActor(
    TSubclassOf<ATargetingActor> TargetingActorClass,
    ETargetTeam TargetTeam,
    float Radius = 300.f
  );
  UFUNCTION(BlueprintCallable)
  void HideTargetingActor();

  void SetShouldOccludeObjects(bool bInShouldOcclude);

  void SetPlayerCamera(UCameraComponent* InCamera) { PlayerCamera = InCamera; }

  UFUNCTION(BlueprintImplementableEvent, Category="Camera|Occlusion")
  void OnOcclusionChange(bool bIsOccluding);

  UUINavPCComponent* GetUINavComponent() const { return UINavPCComp; }
  UUIManager* GetUIManager() const { return UIManager; }

  void AimAbilityGamepad(AActor* AvatarActor, FHitResult& OutHitResult);
  void AimAbilityMouse(AActor* AvatarActor, FHitResult& OutHitResult);

  void AddInteractableInRange(const UInteractComponent* InteractComponent);
  void RemoveInteractableInRange(const UInteractComponent* InteractComponent);

  UFUNCTION(BlueprintCallable)
  void AddInputContext(UInputMappingContext* InputContext, int32 Priority = 0);
  UFUNCTION(BlueprintCallable)
  void RemoveInputContext(UInputMappingContext* InputContext, const FModifyContextOptions& Options = FModifyContextOptions());
  UFUNCTION(BlueprintPure)
  bool HasInputContext(const UInputMappingContext* InputContext);

  UAuraInputConfig* GetInputConfig() const { return InputConfig; }

  UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="ControllerDevice")
  FOnControllerDeviceChanged ControllerDeviceChangedDelegate;

  UPROPERTY(BlueprintAssignable, Category="Interaction")
  FOnInteract InteractActionTriggered;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aim Assist")
  bool bAimAssistOn = true;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Aim Assist",
    meta=(EditCondition="bAimAssistOn", EditConditionHides)
  )
  bool bDebugAimAssist = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadWrite,
    Category="Aim Assist",
    meta=(EditCondition="bAimAssistOn", EditConditionHides)
  )
  float AimAssistStrength = 100.f;

protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

  void HandleEnvironmentOcclusion();
  void GamepadMoveTargetingActor(const FInputActionValue& InputActionValue);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
  TObjectPtr<UUIManager> UIManager;

  UPROPERTY(BlueprintReadWrite, Category="Input")
  bool bUsingGamepad = false;

  UPROPERTY(BlueprintReadOnly, Category="Input")
  FVector InputDirection = FVector::Zero();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
  TObjectPtr<UAuraInputConfig> InputConfig;

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
  TObjectPtr<UInputAction> InteractAction;

  UPROPERTY(EditAnywhere, Category="Input")
  TObjectPtr<UInputAction> CheckInputSourceAction;

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

  void InteractPressed();
  void RemoveLastInteractable();

  void UpdateTargetingActorLocation();
  void UpdatePlayerLocationParameterCollection();
  UAuraAbilitySystemComponent* GetASC();

  UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem();

  UPROPERTY()
  TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;


  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<UHealTextComponent> HealTextComponentClass;

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

  TWeakObjectPtr<UCameraComponent> PlayerCamera;

  bool bControllerEnabled = true;

  UPROPERTY()
  TArray<const UInteractComponent*> InteractablesInRange;

  UPROPERTY()
  TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;
};
