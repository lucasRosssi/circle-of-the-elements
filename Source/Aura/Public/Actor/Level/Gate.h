// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/GatePosition.h"
#include "Enums/Region.h"
#include "Interfaces/InteractInterface.h"
#include "Gate.generated.h"

class UInteractComponent;
class AAuraPlayerController;
class URewardManager;
class ULocationManager;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AGate : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	AGate();

  /* Interact Interface */
  virtual void Interact_Implementation(const AController* Controller) override;
  virtual UInteractComponent* GetInteractComponent_Implementation() const override;
  /* END Interact Interface */

	void SetGateReward(const FGameplayTag& InRewardTag);
	FGameplayTag GetGateReward() const { return RewardTag; }
  bool IsActive() const { return bActive; }
  
	UFUNCTION(BlueprintImplementableEvent)
	void RewardAssigned();

  void DeactivateGate();

protected:
	virtual void BeginPlay() override;
  
  UFUNCTION(BlueprintImplementableEvent)
  void OnInteracted(const AController* Controller);

	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetCurrentLocation();
	
	UFUNCTION(BlueprintPure)
	TSoftObjectPtr<UWorld> GetRandomLocation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GateMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RewardWidget;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Location|Gate")
	ERegion Region = ERegion::Undefined;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Location|Gate")
	EGatePosition NextGatePosition = EGatePosition::BottomLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location|Gate")
	FGameplayTag RewardTag = FGameplayTag();
private:
	UFUNCTION()
	void Enable();

  ULocationManager* GetLocationManager();
  URewardManager* GetRewardManager();
  TWeakObjectPtr<ULocationManager> LocationManager = nullptr;
  TWeakObjectPtr<URewardManager> RewardManager = nullptr;

  bool bActive = true;
};
