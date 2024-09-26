// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Actor/Level/Interactable.h"
#include "Enums/GatePosition.h"
#include "Enums/Region.h"
#include "Gate.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AGate : public AInteractable
{
	GENERATED_BODY()

public:
	AGate();

	void SetGateReward(const FGameplayTag& InRewardTag);
	FGameplayTag GetGateReward() const { return RewardTag; }
  bool IsActive() const { return bActive; }
  
	UFUNCTION(BlueprintImplementableEvent)
	void RewardAssigned();

  void DeactivateGate();

protected:
	virtual void BeginPlay() override;

	// START Interactable overrides
	virtual void Interact(AAuraPlayerController* InstigatorController) override;
	// END Interactable overrides

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLocation();
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetRandomLocation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GateMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RewardWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Location|Gate")
	ERegion Region = ERegion::Undefined;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Location|Gate")
	EGatePosition NextGatePosition = EGatePosition::BottomLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location|Gate")
	FGameplayTag RewardTag = FGameplayTag();
private:
	UFUNCTION()
	void Enable();

  bool bActive = true;
};
