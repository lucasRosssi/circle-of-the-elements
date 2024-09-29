// Copyright Lucas Rossi


#include "Actor/TargetingActor.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Interfaces/TargetInterface.h"

ATargetingActor::ATargetingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingSphere = CreateDefaultSubobject<USphereComponent>("TargetingSphere");
	SetRootComponent(TargetingSphere);
	TargetingSphere->SetCollisionResponseToChannels(ECR_Ignore);
	TargetingSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TargetingSphere->SetUsingAbsoluteRotation(true);
	
	TargetingDecal = CreateDefaultSubobject<UDecalComponent>("TargetingDecal");
	TargetingDecal->SetupAttachment(GetRootComponent());
	TargetingDecal->DecalSize = FVector(TargetingSphere->GetUnscaledSphereRadius());

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	AutoPossessAI = EAutoPossessAI::Spawned;
}

void ATargetingActor::BeginPlay()
{
	Super::BeginPlay();

	TargetingSphere->OnComponentBeginOverlap.AddDynamic(this, &ATargetingActor::OnTargetingBeginOverlap);
	TargetingSphere->OnComponentEndOverlap.AddDynamic(this, &ATargetingActor::OnTargetingEndOverlap);
}

void ATargetingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATargetingActor::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	MovementComponent->AddInputVector(WorldDirection * ScaleValue, bForce);
}

void ATargetingActor::OnTargetingBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (
		TargetTeam == ETargetTeam::Enemies &&
		UAuraAbilitySystemLibrary::AreActorsEnemies(SourceActor, OtherActor)
		)
	{
		ITargetInterface::SafeExec_HighlightActor(OtherActor, SourceActor);
	}

	if (
		TargetTeam == ETargetTeam::Friends &&
		UAuraAbilitySystemLibrary::AreActorsFriends(SourceActor, OtherActor)
		)
	{
		ITargetInterface::SafeExec_HighlightActor(OtherActor, SourceActor);
	}

	if (TargetTeam == ETargetTeam::Both)
	{
		ITargetInterface::SafeExec_HighlightActor(OtherActor, SourceActor);
	}
	
}

void ATargetingActor::OnTargetingEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
	)
{
	ITargetInterface::SafeExec_UnHighlightActor(OtherActor);
}

void ATargetingActor::SetTargetingRadius(float Radius)
{
	TargetingSphere->SetSphereRadius(Radius);
	TargetingDecal->DecalSize = FVector(Radius);
}

