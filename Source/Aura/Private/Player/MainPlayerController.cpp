// Copyright Lucas Rossi


#include "Player/MainPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/TargetInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/DamageTextComponent.h"

AMainPlayerController::AMainPlayerController()
{
	bReplicates = true;
	
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

	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<USpringArmComponent>(
			GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass())
		);
		ActiveCamera = Cast<UCameraComponent>(
			GetPawn()->GetComponentByClass(UCameraComponent::StaticClass())
		);
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(
			GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass())
		);
	}
}

void AMainPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();	
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
	ControllerDeviceChangedDelegate.Broadcast(bIsGamepad);
}

void AMainPlayerController::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;
 
	// Camera is currently colliding, show all current occluded actors
	// and do not perform further occlusion
	if (ActiveSpringArm->bDoCollisionTest)
	{
		ForceShowOccludedActors();
		return;
	}
	
	FVector Start = ActiveCamera->GetComponentLocation();
	FVector NormalizedVector = Start - GetPawn()->GetActorLocation();
	NormalizedVector.Normalize();
	FVector End = GetPawn()->GetActorLocation() + NormalizedVector * DistanceFromTraceEndToCharacter;
	
 
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
 
	TArray<AActor*> ActorsToIgnore; // TODO: Add configuration to ignore actor types
	TArray<FHitResult> OutHits;
 
	auto ShouldDebug = bDebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
 
	bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
		CollisionObjectTypes,
		true,
		ActorsToIgnore,
		ShouldDebug,
		OutHits,
		true
	);
 
	if (bGotHits)
	{
		// The list of actors hit by the line trace, that means that they are occluded from view
		TSet<const AActor*> ActorsJustOccluded;
 
		// Hide actors that are occluded by the camera
		for (FHitResult Hit : OutHits)
		{
			const AActor* HitActor = Cast<AActor>(Hit.GetActor());
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}
 
		// Show actors that are currently hidden but that are not occluded by the camera anymore 
		for (auto& Elem : OccludedActors)
		{
			if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.bIsOccluded)
			{
				ShowOccludedActor(Elem.Value);
 
				if (bDebugLineTraces)
				{
					UE_LOG(
						LogTemp,
						Warning,
						TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."),
						*Elem.Value.Actor->GetName()
					);
				}
			}
		}
	}
	else
	{
		ForceShowOccludedActors();
	}
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
	InputDirection = FVector::Zero();
}

void AMainPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<ITargetInterface>(CursorHit.GetActor());

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
			ThisActor->HighlightActor();
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
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
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
	
}

void AMainPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetASC()) return;
	
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AMainPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetASC()) return;
	
	GetASC()->AbilityInputTagHeld(InputTag);
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

bool AMainPlayerController::HideOccludedActor(const AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);
 
	if (ExistingOccludedActor && ExistingOccludedActor->bIsOccluded)
	{
		if (bDebugLineTraces) UE_LOG(
			LogTemp,
			Warning,
			TEXT("Actor %s was already occluded. Ignoring."),
			*Actor->GetName()
		);
		
		return false;
	}
 
	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->bIsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);
 
		if (bDebugLineTraces) UE_LOG(
			LogTemp,
			Warning,
			TEXT("Actor %s exists, but was not occluded. Occluding it now."),
			*Actor->GetName()
		);
	}
	else
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
			Actor->GetComponentByClass(UStaticMeshComponent::StaticClass())
		);
 
		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMesh = StaticMesh;
		OccludedActor.Materials = StaticMesh->GetMaterials();
		OccludedActor.bIsOccluded = true;
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);
 
		if (bDebugLineTraces) UE_LOG(
			LogTemp,
			Warning,
			TEXT("Actor %s does not exist, creating and occluding it now."),
			*Actor->GetName()
		);
	}
 
	return true;
}

bool AMainPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
	}
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(
		ECC_Visibility,
		ECR_Ignore
	);
 
	return true;
}

void AMainPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if (!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}
 
	OccludedActor.bIsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

bool AMainPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
	{
		OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
	}
	
	OccludedActor.StaticMesh->SetCollisionResponseToChannel(
		ECC_Visibility,
		ECR_Block
	);
	
	return true;
}

void AMainPlayerController::ForceShowOccludedActors()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.bIsOccluded)
		{
			ShowOccludedActor(Elem.Value);
 
			if (bDebugLineTraces) UE_LOG(
				LogTemp,
				Warning,
				TEXT("Actor %s was occluded, force to show again."),
				*Elem.Value.Actor->GetName()
			);
		}
	}
}
