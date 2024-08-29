// Copyright Lucas Rossi


#include "Game/Components/LocationManagerComponent.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"

TSoftObjectPtr<UWorld> ULocationManagerComponent::GetNextLocation(
	ERegion InRegion,
	EGatePosition EntrancePosition
	)
{
	GetAuraGameInstance()->SaveHeroData();

	if (bWillExitRegion)
	{
		return nullptr;
	}

	URegionInfo* RegionInfo = GetAuraGameMode()->RegionInfo;
	TSoftObjectPtr<UWorld> Location;
	if (GetAuraGameMode()->EncountersCount < RegionInfo->GetRegionData(InRegion)->MaxEncounters)
	{
		Location = RegionInfo->GetRandomizedRegionLocation(
		InRegion,
		EntrancePosition, 
	SelectedLocations
		);
	}
	else
	{
		Location = RegionInfo->GetBossArena(InRegion);
		bWillExitRegion = true;
	}
	
	SelectedLocations.Add(Location);
	PrevLocation = CurrentLocation;
	CurrentLocation = Location;

	return Location;
}

TSoftObjectPtr<UWorld> ULocationManagerComponent::GetInitialLocation(ERegion InRegion)
{
	const TSoftObjectPtr<UWorld> Location = GetAuraGameMode()->RegionInfo
		->GetRandomizedInitialLocation(InRegion);
	
	CurrentLocation = Location;
	return Location;
}

void ULocationManagerComponent::PlacePlayerInStartingPoint()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		APlayerStart::StaticClass(),
		PlayerStarts
		);

	if (PlayerStarts.IsEmpty()) return;

	const APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[0]);

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	Player->SetActorTransform(
		PlayerStart->GetActorTransform(),
		false,
		nullptr,
		ETeleportType::ResetPhysics
		);
}

void ULocationManagerComponent::ExitLocation(EGatePosition NextGatePosition)
{
	OnExitLocationDelegate.Broadcast(NextGatePosition);
}

UAuraGameInstance* ULocationManagerComponent::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	}

	return AuraGameInstance;
}
