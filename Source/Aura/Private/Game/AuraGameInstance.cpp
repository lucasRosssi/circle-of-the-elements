// Copyright Lucas Rossi


#include "Game/AuraGameInstance.h"

#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"

void UAuraGameInstance::SaveHeroData()
{
}

void UAuraGameInstance::AddPlayerResource(const FGameplayTag& ResourceTag, int32 Amount)
{
	if (int32* CurrentAmount = PlayerResources.Find(ResourceTag))
	{
		*CurrentAmount += Amount;
	}
	else
	{
		UE_LOG(
			LogAura,
			Error,
			TEXT("Could not find %s resource in the game instance!"),
			*ResourceTag.ToString()
			);
	}
}
