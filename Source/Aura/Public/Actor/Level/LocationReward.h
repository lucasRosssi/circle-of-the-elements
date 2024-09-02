// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actor/Level/Interactable.h"
#include "LocationReward.generated.h"

class UAuraUserWidget;
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

	FGameplayTag GetAbilityElement() const;
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Reward",
		meta=(Categories="Resources")
		)
	FGameplayTag RewardTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Reward|UI")
	bool bShowMenuOnPickup = false;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Reward|UI",
		meta=(EditCondition="bShowMenuOnPickup"))
	TSubclassOf<UAuraUserWidget> MenuWidgetClass;

private:
	UAuraGameInstance* GetAuraGameInstance();
	UPROPERTY()
	UAuraGameInstance* AuraGameInstance = nullptr;
};
