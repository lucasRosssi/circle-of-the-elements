// Copyright Lucas Rossi


#include "Utils/AuraSystemsLibrary.h"

#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"


UAbilityManager* UAuraSystemsLibrary::GetAbilityManager(const UObject* WorldContextObject)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
	{
		return AuraGameMode->GetAbilityManager();
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Current world doesn't have an Ability Manager. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}

ULocationManager* UAuraSystemsLibrary::GetLocationManager(const UObject* WorldContextObject)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
	{
		return AuraGameMode->GetLocationManager();
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Current world doesn't have a Location Manager. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}

UEncounterManager* UAuraSystemsLibrary::GetEncounterManager(const UObject* WorldContextObject)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
	{
		return AuraGameMode->GetEncounterManager();
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Current world doesn't have an Encounter Manager. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}

URewardManager* UAuraSystemsLibrary::GetRewardManager(const UObject* WorldContextObject)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
	{
		return AuraGameMode->GetRewardManager();
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Current world doesn't have a Reward Manager. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}
