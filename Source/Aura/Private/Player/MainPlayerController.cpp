// Copyright Lucas Rossi


#include "Player/MainPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/TargetingActor.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UI/Widget/DamageTextComponent.h"

AMainPlayerController::AMainPlayerController()
{
	bReplicates = true;
}

void AMainPlayerController::ShowTargetingActor(
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
		TargetingActor->AttachToComponent(PlayerCamera, FAttachmentTransformRules::KeepWorldTransform);

		TargetingActor->FinishSpawning(SpawnTransform);
	}
}

void AMainPlayerController::HideTargetingActor()
{
	if (!bUsingGamepad)	bShowMouseCursor = true;
	
	bTargeting = false;
	if (IsValid(TargetingActor))
	{
		TargetingActor->Destroy();
	}
}

void AMainPlayerController::SetShouldOccludeObjects(bool bInShouldOcclude)
{
	if (!OcclusionMaskParameterCollection) return;
	
	GetOcclusionMaskParameterCollectionInstance()->SetScalarParameterValue(
		FName("OcclusionSize"),
		OcclusionSize
		);
}

void AMainPlayerController::BeginPlay()
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

void AMainPlayerController::HandleEnvironmentOcclusion()
{
	if (!PlayerCamera) return;
	
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

void AMainPlayerController::GamepadMoveTargetingActor(const FInputActionValue& InputActionValue)
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

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	UpdateTargetingActorLocation();
	UpdatePlayerLocationParameterCollection();
	HandleEnvironmentOcclusion();
}

void AMainPlayerController::ShowDamageNumber_Implementation(
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

void AMainPlayerController::ChangeUsingGamepad(bool bIsGamepad)
{
	bUsingGamepad = bIsGamepad;
	bShowMouseCursor = !bIsGamepad;
	ControllerDeviceChangedDelegate.Broadcast(bIsGamepad);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>
	(InputComponent);

	AuraInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&AMainPlayerController::Move
	);
	AuraInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Completed,
		this,
		&AMainPlayerController::MoveComplete
	);
	AuraInputComponent->BindAction(
		TargetingActorMoveAction,
		ETriggerEvent::Triggered,
		this,
		&AMainPlayerController::GamepadMoveTargetingActor
	);
	AuraInputComponent->BindAction(
		ConfirmAction,
		ETriggerEvent::Started,
		this,
		&AMainPlayerController::ConfirmPressed
	);
	AuraInputComponent->BindAction(
		CancelAction,
		ETriggerEvent::Started,
		this,
		&AMainPlayerController::CancelPressed
	);

	AuraInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld
	);
}

void AMainPlayerController::Move(const FInputActionValue& InputActionValue)
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

void AMainPlayerController::MoveComplete(const FInputActionValue& InputActionValue)
{
	// InputDirection = FVector::Zero();
}

void AMainPlayerController::CursorTrace()
{
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

void AMainPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagPressed(InputTag);
}

void AMainPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AMainPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagHeld(InputTag);
}

void AMainPlayerController::ConfirmPressed()
{
	GetASC()->ConfirmPressed();
}

void AMainPlayerController::CancelPressed()
{
	GetASC()->CancelPressed();
}

UAuraAbilitySystemComponent* AMainPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		auto ASC = UAbilitySystemBlueprintLibrary::
			GetAbilitySystemComponent(GetPawn<APawn>());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(ASC);

	}
	
	return AuraAbilitySystemComponent;
}

void AMainPlayerController::UpdateTargetingActorLocation()
{
	if (!bUsingGamepad && IsValid(TargetingActor))
	{
		TargetingActor->SetActorLocation(CursorHit.ImpactPoint);
	}
}

UMaterialParameterCollectionInstance* AMainPlayerController::GetOcclusionMaskParameterCollectionInstance()
{
	if (OcclusionMaskParameterCollectionInstance == nullptr)
	{
		OcclusionMaskParameterCollectionInstance = GetWorld()
		->GetParameterCollectionInstance(OcclusionMaskParameterCollection);
	}

	return OcclusionMaskParameterCollectionInstance;
}

void AMainPlayerController::UpdatePlayerLocationParameterCollection()
{
	if (!OcclusionMaskParameterCollection) return;

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
