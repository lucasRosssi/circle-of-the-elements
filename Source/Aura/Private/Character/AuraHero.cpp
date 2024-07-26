// Copyright Lucas Rossi


#include "..\..\Public\Character\AuraHero.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
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

int32 AAuraHero::GetCharacterLevel_Implementation()
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
	MainPlayerController->ShowTargetingActor(TargetingActorClass, TargetTeam, Radius);
}

void AAuraHero::HideTargetingActor_Implementation()
{
	MainPlayerController->HideTargetingActor();
}

AAuraPlayerState* AAuraHero::GetAuraPlayerState() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState;
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

void AAuraHero::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	GetAuraASC()->AbilityActorInfoSet();

	AuraPlayerState->AddSkillPoints(1);

	if (IsLocallyControlled() || HasAuthority())
	{
		MainPlayerController = GetController<AAuraPlayerController>();
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
