// Copyright Lucas Rossi


#include "Actor/Level/Gate.h"

#include "Components/InteractComponent.h"
#include "Components/WidgetComponent.h"
#include "Managers/LocationManager.h"
#include "Managers/RewardManager.h"
#include "Utils/AuraSystemsLibrary.h"

AGate::AGate()
{
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>("GateMesh");
	SetRootComponent(GateMesh);

	RewardWidget = CreateDefaultSubobject<UWidgetComponent>("RewardWidget");
	RewardWidget->SetupAttachment(GetRootComponent());
	RewardWidget->SetVisibility(false);

  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetupInteractAreaAttachment(GetRootComponent());
}

void AGate::SetGateReward(const FGameplayTag& InRewardTag)
{
	RewardTag = InRewardTag;
	RewardAssigned();
}

void AGate::DeactivateGate()
{
  bActive = false;
}

void AGate::BeginPlay()
{
	Super::BeginPlay();

  GetRewardManager()->OnRewardTakenDelegate.AddDynamic(this, &AGate::Enable);
  InteractComponent->OnInteractedDelegate.BindUObject(this, &AGate::Interact);
  InteractComponent->OnPostInteractedDelegate.BindUObject(this, &AGate::OnInteracted);
}

void AGate::Interact(const AAuraPlayerController* InstigatorController)
{
	GetRewardManager()->SetNextReward(RewardTag);
	RewardWidget->SetVisibility(false);
}

TSoftObjectPtr<UWorld> AGate::GetCurrentLocation()
{
	return GetLocationManager()->GetCurrentLocation();
}

TSoftObjectPtr<UWorld> AGate::GetRandomLocation()
{
	return GetLocationManager()->GetNextLocation(Region, NextGatePosition);
}

void AGate::Enable()
{
  if (!bActive) return;
  
	InteractComponent->EnableInteraction();
	RewardWidget->SetVisibility(true);
}

ULocationManager* AGate::GetLocationManager()
{
  if (LocationManager.Get() == nullptr)
  {
    LocationManager = UAuraSystemsLibrary::GetLocationManager(this);
  }

  return LocationManager.Get();
}

URewardManager* AGate::GetRewardManager()
{
  if (RewardManager.Get() == nullptr)
  {
    RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
  }

  return RewardManager.Get();
}
