// Copyright Lucas Rossi


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/TargetingActor.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/InteractComponent.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/TargetInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/UIManager.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	bAutoManageActiveCameraTarget = false;
	
	UIManager = CreateDefaultSubobject<UUIManager>("UIManager");
	UIManager->SetPlayerController(this);
}

void AAuraPlayerController::ShowTargetingActor(
	TSubclassOf<ATargetingActor> TargetingActorClass,
	ETargetTeam TargetTeam,
	float Radius
	)
{
	if (!IsValid(TargetingActor))
	{
		bTargeting = true;
		FTransform SpawnTransform;
		if (bUsingGamepad)
		{
			FVector PawnLocation = GetPawn()->GetActorLocation();
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(
				HitResult,
				PawnLocation + FVector(0.f, 0.f, 400.f),
				PawnLocation - FVector(0.f, 0.f, -400.f),
				ECC_ExcludeCharacters
			);
			
			SpawnTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : PawnLocation);
		}
		else
		{
			bShowMouseCursor = false;
			
			SpawnTransform.SetLocation(CursorHit.ImpactPoint);
		}

		TargetingActor = GetWorld()->SpawnActorDeferred<ATargetingActor>(
			TargetingActorClass,
			SpawnTransform,
			this,
			GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
		TargetingActor->SetSourceActor(GetPawn());
		TargetingActor->SetTargetTeam(TargetTeam);
		TargetingActor->SetTargetingRadius(Radius);
		TargetingActor->AttachToComponent(PlayerCamera.Get(), FAttachmentTransformRules::KeepWorldTransform);

		TargetingActor->FinishSpawning(SpawnTransform);
	}
}

void AAuraPlayerController::HideTargetingActor()
{
	if (!bUsingGamepad)	bShowMouseCursor = true;
	
	bTargeting = false;
	if (IsValid(TargetingActor))
	{
		TargetingActor->Destroy();
	}
}

void AAuraPlayerController::SetShouldOccludeObjects(bool bInShouldOcclude)
{
	if (!OcclusionMaskParameterCollection) return;
	
	GetOcclusionMaskParameterCollectionInstance()->SetScalarParameterValue(
		FName("OcclusionSize"),
		OcclusionSize
		);
}

void AAuraPlayerController::AimAbilityGamepad(AActor* AvatarActor, FHitResult& OutHitResult)
{
	if (!bAimAssistOn || IsTargeting()) return;
	
	FHitResult AimAssistHitResult;
	const TArray ActorsToIgnore({ AvatarActor });
	FVector End = OutHitResult.Location;
	End.Z = AvatarActor->GetActorLocation().Z;
	UKismetSystemLibrary::BoxTraceSingle(
		AvatarActor,
		AvatarActor->GetActorLocation(),
		End,
		FVector(0.f, AimAssistStrength, 15.f),
		UKismetMathLibrary::FindLookAtRotation(
			AvatarActor->GetActorLocation(),
			End
			),
		ETT_AimAssist,
		false,
		ActorsToIgnore,
		bDebugAimAssist ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		AimAssistHitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.f
		);

	if (AimAssistHitResult.bBlockingHit)
	{
		AActor* HitActor = AimAssistHitResult.HitObjectHandle.FetchActor();
		if (HitActor->Implements<UCombatInterface>())
		{
			if (ICombatInterface::Execute_IsEnemy(HitActor, AvatarActor))
			{
				OutHitResult.Location = AimAssistHitResult.HitObjectHandle.FetchActor()->GetActorLocation();
				OutHitResult.ImpactPoint = AimAssistHitResult.HitObjectHandle.FetchActor()->GetActorLocation();
			}
		}
	}
}

void AAuraPlayerController::AimAbilityMouse(AActor* AvatarActor, FHitResult& OutHitResult)
{
	if (!bAimAssistOn || IsTargeting()) return;
	
	const AActor* MouseHitActor = OutHitResult.HitObjectHandle.FetchActor();
	if (!MouseHitActor->Implements<UCombatInterface>())
	{
		const float AimAssistStrengthShapeCompensation = AimAssistStrength * 2.f;
		TArray<AActor*> ActorsToIgnore;
		const AActor* Target = UAuraAbilitySystemLibrary::GetClosestTargetToPoint(
			AvatarActor,
			OutHitResult.Location,
			AimAssistStrengthShapeCompensation,
			ETargetTeam::Enemies,
			ActorsToIgnore
			);
		
		if (bDebugAimAssist)
		{
			UKismetSystemLibrary::DrawDebugSphere(
				AvatarActor,
				OutHitResult.Location,
				AimAssistStrengthShapeCompensation,
				12,
				FLinearColor::Red,
				2.f,
				1.f
				);
			if (Target)
			{
				UKismetSystemLibrary::DrawDebugSphere(
					Target,
					Target->GetActorLocation(),
					50.f,
					12,
					FLinearColor::Green,
					2.f,
					2.f
					);
			}
		}

		if (Target)
		{
			OutHitResult.Location = Target->GetActorLocation();
			OutHitResult.ImpactPoint = Target->GetActorLocation();
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(MainContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = 
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(MainContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::HandleEnvironmentOcclusion()
{
	if (!PlayerCamera.Get() || !bControllerEnabled) return;
	
	const TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		PlayerCamera->GetComponentLocation(),
		GetPawn()->GetActorLocation(),
		GetPawn()->GetSimpleCollisionRadius() - 5.f,
		GetPawn()->GetSimpleCollisionHalfHeight(),
		ETT_CameraToPlayer,
		false,
		ActorsToIgnore,
		bDebugCameraOcclusionTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	if (HitResult.bBlockingHit && !bCameraOcclusionActive)
	{
		bCameraOcclusionActive = true;
		OnOcclusionChange(true);
	}
	else if (!HitResult.bBlockingHit && bCameraOcclusionActive)
	{
		bCameraOcclusionActive = false;
		OnOcclusionChange(false);
	}
}

void AAuraPlayerController::GamepadMoveTargetingActor(const FInputActionValue& InputActionValue)
{
	if (!bTargeting || !IsValid(TargetingActor)) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	InputDirection = ForwardDirection * InputAxisVector.Y + RightDirection * InputAxisVector.X;
	InputDirection.Normalize();
	
	TargetingActor->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	TargetingActor->AddMovementInput(RightDirection, InputAxisVector.X);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	UpdateTargetingActorLocation();
	UpdatePlayerLocationParameterCollection();
	HandleEnvironmentOcclusion();
}

void AAuraPlayerController::EnableController()
{
	bControllerEnabled = true;
	EnableInput(this);
}

void AAuraPlayerController::DisableController()
{
	bControllerEnabled = false;
	DisableInput(this);
	LastActor = nullptr;
	ThisActor = nullptr;
}

void AAuraPlayerController::ShowDamageNumber_Implementation(
		float DamageAmount,
		ACharacter* TargetCharacter,
		bool bParried,
		bool bCriticalHit,
		bool bIsPlayer
	)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(
			TargetCharacter,
			DamageTextComponentClass
		);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(
			TargetCharacter->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bParried, bCriticalHit, bIsPlayer);
	}
}

void AAuraPlayerController::ChangeUsingGamepad(bool bIsGamepad)
{
	bUsingGamepad = bIsGamepad;
	bShowMouseCursor = !bIsGamepad;
	ControllerDeviceChangedDelegate.Broadcast(bIsGamepad);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>
	(InputComponent);

	AuraInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&AAuraPlayerController::Move
	);
	AuraInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Completed,
		this,
		&AAuraPlayerController::MoveComplete
	);
	AuraInputComponent->BindAction(
		TargetingActorMoveAction,
		ETriggerEvent::Triggered,
		this,
		&AAuraPlayerController::GamepadMoveTargetingActor
	);
	AuraInputComponent->BindAction(
		ConfirmAction,
		ETriggerEvent::Started,
		this,
		&AAuraPlayerController::ConfirmPressed
	);
	AuraInputComponent->BindAction(
		CancelAction,
		ETriggerEvent::Started,
		this,
		&AAuraPlayerController::CancelPressed
	);
	AuraInputComponent->BindAction(
		InteractAction,
		ETriggerEvent::Started,
		this,
		&AAuraPlayerController::InteractPressed
	);

	AuraInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld
	);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	InputDirection = ForwardDirection * InputAxisVector.Y + RightDirection * InputAxisVector.X;
	InputDirection.Normalize();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::MoveComplete(const FInputActionValue& InputActionValue)
{
	// InputDirection = FVector::Zero();
}

void AAuraPlayerController::CursorTrace()
{
	if (!bControllerEnabled) return;
	
	const ECollisionChannel TraceChannel = bTargeting ? ECC_ExcludeCharacters : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *	- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *  - Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *  - Unhighlight LastActor
	 * D. Both actors are valid, but LastActor != ThisActor
	 *  - Unhighlight LastActor and Highlight ThisActor
	 * E. Both actors are valid, and are the same
	 *  - Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ITargetInterface::SafeExec_HighlightActor(ThisActor, GetPawn());
		}
		else // ThisActor == nullptr
		{
			// Case A
			// Do nothing
		}
	}
	else // LastActor != nullptr
	{
		if (ThisActor == nullptr)
		{
			// Case C
			ITargetInterface::SafeExec_UnHighlightActor(LastActor);
		}
		else
		{
			if (LastActor != ThisActor)
			{
				// Case D
				ITargetInterface::SafeExec_UnHighlightActor(LastActor);
				ITargetInterface::SafeExec_HighlightActor(ThisActor, GetPawn());
			}
			else
			{
				// Case E
				// Do nothing
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagHeld(InputTag);
}

void AAuraPlayerController::ConfirmPressed()
{
	GetASC()->ConfirmPressed();
}

void AAuraPlayerController::CancelPressed()
{
	GetASC()->CancelPressed();
}

void AAuraPlayerController::InteractPressed()
{
	if (InteractablesInRange.IsEmpty()) return;
  
  const UInteractComponent* Interactable = InteractablesInRange[InteractablesInRange.Num() - 1];
  if (!IsValid(Interactable))
  {
    RemoveInteractableInRange(Interactable);
    return;
  }

  if (!Interactable->IsEnabled()) return;

  Interactable->BeginInteract(this);
}

void AAuraPlayerController::AddInteractableInRange(const UInteractComponent* InteractComponent)
{
  if (!IsValid(InteractComponent)) return;

  InteractablesInRange.Add(InteractComponent);
  
  if (InteractablesInRange.Num() == 1)
  {
    IPlayerInterface::Safe_SetInteractMessageVisible(GetPawn(), InteractComponent->GetInteractText());
  }
}

void AAuraPlayerController::RemoveInteractableInRange(const UInteractComponent* InteractComponent)
{
  InteractablesInRange.Remove(InteractComponent);
  
  if (InteractablesInRange.Num() == 0)
  {
    IPlayerInterface::Safe_SetInteractMessageVisible(GetPawn(), FText());
  }
  else
  {
    IPlayerInterface::Safe_SetInteractMessageVisible(
      GetPawn(),
      InteractablesInRange[InteractablesInRange.Num() - 1]->GetInteractText()
      );
  }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		auto ASC = UAbilitySystemBlueprintLibrary::
			GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(ASC);

	}
	
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::UpdateTargetingActorLocation()
{
	if (bControllerEnabled && !bUsingGamepad && IsValid(TargetingActor))
	{
		TargetingActor->SetActorLocation(CursorHit.ImpactPoint);
	}
}

UMaterialParameterCollectionInstance* AAuraPlayerController::GetOcclusionMaskParameterCollectionInstance()
{
	if (OcclusionMaskParameterCollectionInstance == nullptr)
	{
		OcclusionMaskParameterCollectionInstance = GetWorld()
		->GetParameterCollectionInstance(OcclusionMaskParameterCollection);
	}

	return OcclusionMaskParameterCollectionInstance;
}

void AAuraPlayerController::UpdatePlayerLocationParameterCollection()
{
	if (!OcclusionMaskParameterCollection || !bControllerEnabled) return;

	const FVector PlayerLocation = GetPawn()->GetActorLocation();
	const FLinearColor ColorPlayerLocation = FLinearColor(
		PlayerLocation.X,
		PlayerLocation.Y, 
		PlayerLocation.Z
		);
	GetOcclusionMaskParameterCollectionInstance()->SetVectorParameterValue(
		FName("PlayerLocation"),
		ColorPlayerLocation
		);
}
