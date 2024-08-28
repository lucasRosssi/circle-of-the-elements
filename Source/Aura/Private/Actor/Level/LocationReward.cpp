// Copyright Lucas Rossi


#include "Actor/Level/LocationReward.h"

#include "AuraGameplayTags.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Level/RewardsInfo.h"
#include "Player/AuraPlayerState.h"

ALocationReward::ALocationReward()
{
	EnableInteraction();
}

void ALocationReward::Interact(AController* InstigatorController)
{
	Super::Interact(InstigatorController);
	
	const FRewardInfo& RewardInfo = GetAuraGameMode()->RewardsInfo->GetRewardInfo(RewardTag);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	if (RewardTag.MatchesTag(GameplayTags.Resources_Point))
	{
		AAuraPlayerState* AuraPS = InstigatorController->GetPlayerState<AAuraPlayerState>();

		if (RewardTag.MatchesTagExact(GameplayTags.Resources_Point_Attribute))
		{
			AuraPS->AddAttributePoints(RewardInfo.Amount);
		}
		else if (RewardTag.MatchesTagExact(GameplayTags.Resources_Point_Skill))
		{
			AuraPS->AddSkillPoints(RewardInfo.Amount);
		}
		else
		{
			AuraPS->AddPerkPoints(RewardInfo.Amount);
		}
	}
	else
	{
		GetAuraGameInstance()->AddPlayerResource(RewardTag, RewardInfo.Amount);
	}

	Destroy();
}

UAuraGameInstance* ALocationReward::GetAuraGameInstance()
{
	if (AuraGameInstance == nullptr)
	{
		AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	}

	return AuraGameInstance;
}
