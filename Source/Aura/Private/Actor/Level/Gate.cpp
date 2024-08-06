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

void AGate::Interact()
{
	GoToRandomLocation();
}

void AGate::GoToRandomLocation()
{
	GetAuraGameMode()->GoToLocation(Region, NextGatePosition);
}
