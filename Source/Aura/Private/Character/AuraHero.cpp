// Copyright Lucas Rossi


#include "..\..\Public\Character\AuraHero.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Game/TeamComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraHero::AAuraHero()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 5000.f;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetFieldOfView(30.f);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
	SpotLight->SetupAttachment(GetRootComponent());
	SpotLight->Intensity = 10000.f;
	SpotLight->AttenuationRadius = 2000.f;
	SpotLight->SetUseTemperature(true);
	SpotLight->Temperature = 5000.f;
	SpotLight->CastShadows = false;
	SpotLight->bAffectsWorld = false;
	
	LevelUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("LevelUpMessage");
	LevelUpWidgetComponent->SetupAttachment(GetRootComponent());
	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InteractMessage");
	InteractWidgetComponent->SetupAttachment(GetRootComponent());
	InteractWidgetComponent->SetVisibility(false);
	
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, 400.f, 0.f);
	Movement->bConstrainToPlane = true;
	Movement->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterType = ECharacterType::Player;
	TeamComponent->TeamID = PLAYER_TEAM;
}

void AAuraHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bDying)
	{
		Camera->SetFieldOfView(FMath::FInterpTo(
			Camera->FieldOfView,
			15.f,
			DeltaSeconds,
			10.f
			));
	}
}

void AAuraHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}
	
void AAuraHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAuraHero::AddCharacterAbilities()
{
	Super::AddCharacterAbilities();

	GetAuraASC()->AddCharacterAbilities(
		EligibleAbilities,
		FAuraGameplayTags::Get().Abilities_Status_Eligible
		);
}

void AAuraHero::Die(const FVector& DeathImpulse)
{
	bDying = true;

	StartDeath();
}

void AAuraHero::DeathMontageEndRagdoll()
{
	RagdollMesh();
}

int32 AAuraHero::GetCharacterLevel_Implementation() const
{
	return GetAuraPlayerState()->GetCharacterLevel();
}

void AAuraHero::AddToXP_Implementation(int32 InXP)
{
	GetAuraPlayerState()->AddXP(InXP);
}

int32 AAuraHero::GetXP_Implementation() const
{
	return GetAuraPlayerState()->GetXP();;
}

void AAuraHero::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraHero::GetAttributePoints_Implementation() const
{
	return GetAuraPlayerState()->GetAttributePoints();
}

int32 AAuraHero::GetSkillPoints_Implementation() const
{
	return GetAuraPlayerState()->GetSkillPoints();
}

void AAuraHero::SpendAttributePoints_Implementation(int32 Amount)
{
	GetAuraPlayerState()->AddAttributePoints(-Amount);
}

void AAuraHero::SpendSkillPoints_Implementation(int32 Amount)
{
	GetAuraPlayerState()->AddSkillPoints(-Amount);
}

void AAuraHero::ShowTargetingActor_Implementation(
	TSubclassOf<ATargetingActor> TargetingActorClass,
	ETargetTeam TargetTeam,
	float Radius
	)
{
	AuraPlayerController->ShowTargetingActor(TargetingActorClass, TargetTeam, Radius);
}

void AAuraHero::HideTargetingActor_Implementation()
{
	AuraPlayerController->HideTargetingActor();
}

FOnInteract& AAuraHero::GetOnInteractDelegate()
{
	return GetAuraPlayerController()->InteractActionTriggered;
}

void AAuraHero::SetInteractMessageVisible_Implementation(bool bVisible)
{
	InteractWidgetComponent->SetVisibility(bVisible);
}

void AAuraHero::StartDeath()
{
	bDead = true;
	OnDeath.Broadcast(this);
	
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActor(this);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GetWorld())
	{
		World->OverlapMultiByObjectType(
			Overlaps,
			GetActorLocation(),
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects),
			FCollisionShape::MakeSphere(500.f),
			SphereParams
		);
		for (auto& Overlap : Overlaps)
		{
			AActor* OverlappedActor = Overlap.GetActor();

			if (!IsValid(OverlappedActor)) continue;

			OverlappedActor->SetActorHiddenInGame(true);
		}
	}
	
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEATH);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEATH);
	
	APostProcessVolume* PPDeath = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(this, APostProcessVolume::StaticClass())
		);

	PPDeath->Settings.WeightedBlendables.Array[0].Weight = 0.f;
	PPDeath->Settings.WeightedBlendables.Array[1].Weight = 1.f;
	
	GetMovementComponent()->Deactivate();
	SpotLight->bAffectsWorld = true;
	
	UGameplayStatics::SetGlobalTimeDilation(this, 0.25f);
	PlayAnimMontage(HitReactMontage, 0.66f);

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(
		DeathTimer,
		this,
		&AAuraHero::EndDeath,
		0.3f,
		false
		);
}

void AAuraHero::EndDeath()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEATH_DEFAULT);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEATH_DEFAULT);
	
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		DeathBloodEffect,
		GetActorLocation()
		);
	NiagaraComponent->SetRenderCustomDepth(true);
	NiagaraComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_DEATH_DEFAULT);
	NiagaraComponent->MarkRenderStateDirty();

	UGameplayStatics::PlaySound2D(
		this,
		DeathSound1,
		1.f,
		0.7f
		);

	UGameplayStatics::PlaySound2D(
		this,
		DeathSound2,
		1.f,
		0.7f
		);

	UGameplayStatics::SetGlobalTimeDilation(this, 1.f);

	if (DeathMontage != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(
		this,
		DeathSound,
		GetActorLocation(),
		GetActorRotation()
	);
		
		ReleaseWeapon();
		const float MontageLength = PlayAnimMontage(DeathMontage);

		FTimerHandle MontageTimer;
		GetWorld()->GetTimerManager().SetTimer(
			MontageTimer,
			this,
			&AAuraHero::DeathMontageEndRagdoll,
			MontageLength,
			false
			);
	}
	else
	{
		Super::Die();
	}
}

AAuraPlayerState* AAuraHero::GetAuraPlayerState() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState;
}

AAuraPlayerController* AAuraHero::GetAuraPlayerController()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(GetController());
	}

	return AuraPlayerController;
}

void AAuraHero::MulticastLevelUpParticles_Implementation() const
{
	LevelUpWidgetComponent->GetWidget()->SetVisibility(ESlateVisibility::Visible);
	if (IsLocallyControlled() && IsValid(LevelUpSound))
	{
		UGameplayStatics::PlaySound2D(
			this,
			LevelUpSound
		);
	}
	if (IsValid(LevelUpNiagaraSystem))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			LevelUpNiagaraSystem,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void AAuraHero::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	AttributeSet->AssignPrimeAttribute(PrimeAttributeTag);
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	GetAuraASC()->AbilityActorInfoSet();

	AuraPlayerState->AddSkillPoints(1);

	if (IsLocallyControlled() || HasAuthority())
	{
		if (GetAuraPlayerController())
		{
			AuraPlayerController->SetPlayerCamera(Camera);
		}
		
	}
	
	if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
	{
		AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, 
		AttributeSet);
	}
	
	InitializeDefaultAttributes();
}
