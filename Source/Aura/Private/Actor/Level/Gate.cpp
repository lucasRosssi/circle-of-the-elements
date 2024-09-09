// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Components/WidgetComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/Components/LocationManager.h"

AGate::AGate()
{
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
	GateMesh->SetupAttachment(GetRootComponent());

	RewardWidget = CreateDefaultSubobject<UWidgetComponent>("RewardWidget");
	RewardWidget->SetupAttachment(GetRootComponent());
	RewardWidget->SetVisibility(false);
}

void AGate::SetGateReward(const FGameplayTag& InRewardTag)
{
	RewardTag = InRewardTag;
	RewardAssigned();
}

void AGate::BeginPlay()
{
	Super::BeginPlay();

	GetAuraGameMode()->GetOnRewardTakenDelegate().AddDynamic(this, &AGate::Enable);
}

void AGate::Interact(AAuraPlayerController* InstigatorController)
{
	Super::Interact(InstigatorController);

	GetAuraGameMode()->SetNextReward(RewardTag);
	RewardWidget->SetVisibility(false);
}

TSoftObjectPtr<UWorld> AGate::GetCurrentLocation()
{
	return GetAuraGameMode()->GetLocationManager()->GetCurrentLocation();
}

TSoftObjectPtr<UWorld> AGate::GetRandomLocation()
{
	return GetAuraGameMode()->GetLocationManager()->GetNextLocation(Region, NextGatePosition);
}

void AGate::Enable()
{
	EnableInteraction();
	RewardWidget->SetVisibility(true);
}
