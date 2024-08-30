// Copyright Lucas Rossi


#include "Actor/Level/LocationReward.h"

#include "AuraGameplayTags.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/Components/RewardManager.h"
#include "Level/RewardsInfo.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"

ALocationReward::ALocationReward()
{
	EnableInteraction();
}

void ALocationReward::Interact(AController* InstigatorController)
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
			UAuraAbilitySystemLibrary
				::GetOverlayWidgetController(this)
				->OnElementalEssenceTaken(RewardTag);
		}
		
		GetAuraGameInstance()->AddPlayerResource(RewardTag, RewardInfo.Amount);
	}

	const URewardManager* RewardManager = UAuraSystemsLibrary::GetRewardManager(this);
	RewardManager->OnRewardTakenDelegate.Broadcast();

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
