// Copyright Lucas Rossi


#include "Actor/TargetingActor.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/TargetInterface.h"

ATargetingActor::ATargetingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingSphere = CreateDefaultSubobject<USphereComponent>("TargetingSphere");
	SetRootComponent(TargetingSphere);
	TargetingSphere->SetCollisionResponseToChannels(ECR_Ignore);
	TargetingSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	TargetingDecal = CreateDefaultSubobject<UDecalComponent>("TargetingDecal");
	TargetingDecal->SetupAttachment(GetRootComponent());
	TargetingDecal->DecalSize = FVector(TargetingSphere->GetUnscaledSphereRadius());
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

void ATargetingActor::OnTargetingBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (ITargetInterface* Target = Cast<ITargetInterface>(OtherActor))
	{
		if (TargetTeam == ETargetTeam::Enemies &&
			UAuraAbilitySystemLibrary::AreActorsEnemies(SourceActor, OtherActor)
			)
		{
			Target->HighlightActor(SourceActor);
		}

		if (TargetTeam == ETargetTeam::Friends &&
			UAuraAbilitySystemLibrary::AreActorsFriends(SourceActor, OtherActor)
			)
		{
			Target->HighlightActor(SourceActor);
		}

		if (TargetTeam == ETargetTeam::Both)
		{
			Target->HighlightActor(SourceActor);
		}
	}
}

void ATargetingActor::OnTargetingEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
	)
{
	if (ITargetInterface* Target = Cast<ITargetInterface>(OtherActor))
	{
		Target->UnHighlightActor();
	}
}

void ATargetingActor::SetTargetingRadius(float Radius)
{
	TargetingSphere->SetSphereRadius(Radius);
	TargetingDecal->DecalSize = FVector(Radius);
}

