// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "Actor/Level/Interactable.h"
#include "LocationReward.generated.h"

class UAuraGameInstance;
/**
 * 
 */
UCLASS()
class AURA_API ALocationReward : public AInteractable
{
	GENERATED_BODY()

public:
	ALocationReward();

protected:
	// START Interactable overrides
	virtual void Interact(AController* InstigatorController) override;
	// END Interactable overrides
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Reward",
		meta=(Categories="Resources")
		)
	FGameplayTag RewardTag = FGameplayTag();

private:
	UAuraGameInstance* GetAuraGameInstance();
	UPROPERTY()
	UAuraGameInstance* AuraGameInstance = nullptr;
};
