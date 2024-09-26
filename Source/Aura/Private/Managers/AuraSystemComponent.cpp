// Copyright Lucas Rossi


#include "Managers/AuraSystemComponent.h"

#include "Kismet/GameplayStatics.h"

UAuraSystemComponent::UAuraSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

AAuraGameModeBase* UAuraSystemComponent::GetAuraGameMode()
{
	if (AuraGameMode == nullptr)
	{
		AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(GetOwner()));
	}

	return AuraGameMode.Get();
}
