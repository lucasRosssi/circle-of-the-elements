// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interfaces/InteractInterface.h"
#include "LocationReward.generated.h"

class AAuraPlayerController;
class UNiagaraComponent;
class UNiagaraSystem;
class UInteractComponent;
class UAuraUserWidget;
class UAuraGameInstance;
/**
 * 
 */
UCLASS()
class AURA_API ALocationReward : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ALocationReward();

  /* Interact Interface */
  virtual void Interact_Implementation(const AController* Controller) override;
  virtual UInteractComponent* GetInteractComponent_Implementation() const override;
  /* END Interact Interface */
protected:
  virtual void BeginPlay() override;
  virtual void Destroyed() override;
  
	FGameplayTag GetAbilityElement() const;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> RewardMesh;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;
	
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

  UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Spawn")
  TObjectPtr<UNiagaraSystem> SpawnNiagaraSystem;
  UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Idle")
  TObjectPtr<UNiagaraSystem> IdleNiagaraSystem;
  UPROPERTY()
  UNiagaraComponent* IdleNiagaraComponent = nullptr;

  UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Spawn")
  FVector SpawnNiagaraOffset = FVector::ZeroVector;
  UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Idle")
  FVector IdleNiagaraOffset = FVector::ZeroVector;

private:
  void SpawnNiagaraEffects();

	UAuraGameInstance* GetAuraGameInstance();
	TWeakObjectPtr<UAuraGameInstance> AuraGameInstance = nullptr;
};
