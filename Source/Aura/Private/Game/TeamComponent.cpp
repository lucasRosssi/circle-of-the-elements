// Copyright Lucas Rossi


#include "Game/TeamComponent.h"

// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	TeamID = 255;
}


// Called when the game starts
void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UTeamComponent::IsEnemy(AActor* TargetActor)
{
	if (TeamID == 255) return false;
	
	if (const UTeamComponent* TargetTeamComponent = TargetActor->GetComponentByClass<UTeamComponent>())
	{
		return TargetTeamComponent->TeamID != TeamID;
	}

	return false;
}

bool UTeamComponent::IsFriend(AActor* TargetActor)
{
	if (TeamID == 255) return true;

	if (const UTeamComponent* TargetTeamComponent = TargetActor->GetComponentByClass<UTeamComponent>())
	{
		return TargetTeamComponent->TeamID == TeamID;
	}

	return true;
}

