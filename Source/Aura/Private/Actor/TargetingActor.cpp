// Copyright Lucas Rossi


#include "Actor/TargetingActor.h"

#include "Components/DecalComponent.h"

ATargetingActor::ATargetingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingDecal = CreateDefaultSubobject<UDecalComponent>("TargetingDecal");
	TargetingDecal->SetupAttachment(GetRootComponent());
}

void ATargetingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

