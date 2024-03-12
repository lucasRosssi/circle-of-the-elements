// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "Player/MainPlayerController.h"

bool UAuraGameplayAbility::IsUsingGamepad()
{
	AController* Controller = GetAvatarActorFromActorInfo()->GetInstigatorController();

	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Controller))
	{
		return PlayerController->GetUsingGamepad();
	}

	return false;
}
