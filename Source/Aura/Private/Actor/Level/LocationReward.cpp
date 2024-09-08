// Copyright Lucas Rossi


#include "Actor/Level/LocationReward.h"

#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/Components/RewardManager.h"
#include "Game/Components/UIManager.h"
#include "Level/RewardsInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

ALocationReward::ALocationReward()
{
	EnableInteraction();
}

void ALocationReward::Interact(AAuraPlayerController* InstigatorController)
{
	Super::Interact(InstigatorController);
	
	const FRewardInfo& RewardInfo = GetAuraGameMode()->RewardsInfo->GetRewardInfo(RewardTag);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (RewardTag.MatchesTag(GameplayTags.Resources_Essence))
	{
		AAuraPlayerState* AuraPS = InstigatorController->GetPlayerState<AAuraPlayerState>();

		if (RewardTag.MatchesTagExact(GameplayTags.Resources_Essence_Soul))
		{
			AuraPS->AddAttributePoints(RewardInfo.Amount);
		}
		else
		{
			InstigatorController->GetUIManager()->OpenSkillSelectionMenu.Broadcast(GetAbilityElement());
		}
		
		GetAuraGameInstance()->AddPlayerResource(RewardTag, RewardInfo.Amount);
	}

	const URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
	RewardManager->OnRewardTakenDelegate.Broadcast();
}

FGameplayTag ALocationReward::GetAbilityElement() const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	if (!RewardTag.MatchesTag(AuraTags.Resources_Essence))
	{
		UE_LOG(
			LogAura,
			Error,
			TEXT(
				"Reward must be an Essence to get its corresponding Ability Element."
				"RewardTag is: %s"
				),
				*RewardTag.ToString()
			);
		return FGameplayTag();
	}

	return AuraTags.EssenceToAbility[RewardTag];
}

UAuraGameInstance* ALocationReward::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	}

	return AuraGameInstance.Get();
}
