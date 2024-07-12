// Copyright Lucas Rossi


#include "Game/TeamComponent.h"

#include "Aura/Aura.h"

UTeamComponent::UTeamComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TeamID = NEUTRAL_TEAM;
	OriginalTeamID = NEUTRAL_TEAM;
}

bool UTeamComponent::IsEnemy(AActor* TargetActor)
{
	if (TeamID == NEUTRAL_TEAM) return false;
	
	if (const UTeamComponent* TargetTeamComponent = TargetActor->GetComponentByClass<UTeamComponent>())
	{
		return TargetTeamComponent->TeamID != TeamID;
	}

	return false;
}

bool UTeamComponent::IsFriend(AActor* TargetActor)
{
	if (TeamID == NEUTRAL_TEAM) return false;

	if (const UTeamComponent* TargetTeamComponent = TargetActor->GetComponentByClass<UTeamComponent>())
	{
		return TargetTeamComponent->TeamID == TeamID;
	}

	return true;
}

void UTeamComponent::JoinActorTeam(AActor* Actor, bool bPermanent)
{
	if (const UTeamComponent* ActorTeamComponent = Actor->GetComponentByClass<UTeamComponent>())
	{
		TeamID = ActorTeamComponent->TeamID;
		if (bPermanent)
		{
			OriginalTeamID = TeamID;
		}
	}
}

void UTeamComponent::ChangeTeam(int32 InTeamID, bool bPermanent)
{
	TeamID = InTeamID;
	if (bPermanent)
	{
		OriginalTeamID = TeamID;
	}
}

void UTeamComponent::GoBackToOriginalTeam()
{
	TeamID = OriginalTeamID;
}

void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalTeamID = TeamID;
}

