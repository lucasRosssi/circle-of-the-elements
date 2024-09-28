// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enums/GatePosition.h"
#include "Enums/Region.h"
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
class AURA_API AGate : public AActor
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
  
	void Interact(const AAuraPlayerController* InstigatorController);
  UFUNCTION(BlueprintImplementableEvent)
  void OnInteracted(const AAuraPlayerController* InstigatorController);

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
