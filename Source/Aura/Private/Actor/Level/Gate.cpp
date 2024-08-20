// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Game/AuraGameModeBase.h"

AGate::AGate()
{
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
	GateMesh->SetupAttachment(GetRootComponent());
}

void AGate::BeginPlay()
{
	Super::BeginPlay();

	GetAuraGameMode()->OnEncounterFinishedDelegate.AddDynamic(this, &AGate::EnableInteraction);
}

void AGate::Interact(AController* InstigatorController)
{
	// GoToRandomLocation();
}

TSoftObjectPtr<UWorld> AGate::GetCurrentLocation()
{
	return GetAuraGameMode()->GetCurrentLevel();
}

TSoftObjectPtr<UWorld> AGate::GetRandomLocation()
{
	return GetAuraGameMode()->GetNextLocation(Region, NextGatePosition);
}
