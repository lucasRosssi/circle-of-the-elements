// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Game/AuraGameModeBase.h"
#include "Game/Components/LocationManager.h"

AGate::AGate()
{
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
	GateMesh->SetupAttachment(GetRootComponent());
}

void AGate::BeginPlay()
{
	Super::BeginPlay();

	GetAuraGameMode()->GetOnRewardTakenDelegate().AddDynamic(this, &AGate::EnableInteraction);
}

void AGate::Interact(AAuraPlayerController* InstigatorController)
{
	Super::Interact(InstigatorController);

	GetAuraGameMode()->SetNextReward(RewardTag);
}

TSoftObjectPtr<UWorld> AGate::GetCurrentLocation()
{
	return GetAuraGameMode()->GetLocationManager()->GetCurrentLocation();
}

TSoftObjectPtr<UWorld> AGate::GetRandomLocation()
{
	return GetAuraGameMode()->GetLocationManager()->GetNextLocation(Region, NextGatePosition);
}
