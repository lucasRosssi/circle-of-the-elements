// Copyright Lucas Rossi


#include "Game/Components/AuraSystemComponent.h"

#include "Kismet/GameplayStatics.h"

UAuraSystemComponent::UAuraSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

AAuraGameModeBase* UAuraSystemComponent::GetAuraGameMode()
{
	if (AuraGameMode == nullptr)
	{
		UGameplayStatics::GetGameMode(GetOwner());
	}

	return AuraGameMode;
}
