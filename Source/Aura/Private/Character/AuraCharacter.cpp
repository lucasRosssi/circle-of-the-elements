// Copyright Lucas Rossi


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/TeamComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "Player/MainPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 4000.f;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetFieldOfView(30.f);
	
	LevelUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("LevelUpMessage");
	LevelUpWidgetComponent->SetupAttachment(GetRootComponent());
	
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

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}
	
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetCharacterLevel_Implementation()
{
	return GetAuraPlayerState()->GetCharacterLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	GetAuraPlayerState()->AddXP(InXP);
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	return GetAuraPlayerState()->GetXP();;
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	return GetAuraPlayerState()->GetAttributePoints();
}

int32 AAuraCharacter::GetSkillPoints_Implementation() const
{
	return GetAuraPlayerState()->GetSkillPoints();
}

void AAuraCharacter::SpendAttributePoints_Implementation(int32 Amount)
{
	GetAuraPlayerState()->AddAttributePoints(-Amount);
}

void AAuraCharacter::SpendSkillPoints_Implementation(int32 Amount)
{
	GetAuraPlayerState()->AddSkillPoints(-Amount);
}

void AAuraCharacter::ShowTargetingActor_Implementation(
	TSubclassOf<ATargetingActor> TargetingActorClass,
	ETargetTeam TargetTeam,
	float Radius
	)
{
	MainPlayerController->ShowTargetingActor(TargetingActorClass, TargetTeam, Radius);
}

void AAuraCharacter::HideTargetingActor_Implementation()
{
	MainPlayerController->HideTargetingActor();
}

AAuraPlayerState* AAuraCharacter::GetAuraPlayerState() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState;
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
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
		FRotator Rotation = GetActorRotation();
		Rotation.Pitch = 90.f;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			LevelUpNiagaraSystem,
			GetActorLocation(),
			Rotation
		);
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>
	(AbilitySystemComponent);

	AuraASC->AbilityActorInfoSet();

	if (IsLocallyControlled() || HasAuthority())
	{
		MainPlayerController = GetController<AMainPlayerController>();
		check(MainPlayerController);
		
		MainPlayerController->SetPlayerCamera(Camera);
	}
	
	if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(MainPlayerController->GetHUD()))
	{
		AuraHUD->InitOverlay(MainPlayerController, AuraPlayerState, AbilitySystemComponent, 
		AttributeSet);
	}
	

	InitializeDefaultAttributes();
}
