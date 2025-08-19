// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interfaces/InteractInterface.h"
#include "LocationReward.generated.h"

class USphereComponent;
class UCapsuleComponent;
class AAuraPlayerState;
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
  virtual FGameplayEventData GetAbilityEventData_Implementation() const override;
  /* END Interact Interface */

  UStaticMeshComponent* GetMesh() const { return RewardMesh; }
  UCapsuleComponent* GetCapsule() const { return CapsuleComponent; }
  FGameplayTag GetAbilityElement() const;
protected:
  virtual void BeginPlay() override;
  virtual void Destroyed() override;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> RewardMesh;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UNiagaraComponent> NiagaraComponent;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UCapsuleComponent> CapsuleComponent;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<USphereComponent> InteractSphere;
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
    meta=(EditCondition="bShowMenuOnPickup")
  )
  TSubclassOf<UAuraUserWidget> MenuWidgetClass;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Niagara|Spawn")
  TObjectPtr<UNiagaraSystem> SpawnNiagaraSystem;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Niagara|Idle")
  TObjectPtr<UNiagaraSystem> IdleNiagaraSystem;
  UPROPERTY()
  UNiagaraComponent* IdleNiagaraComponent = nullptr;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Niagara|Spawn")
  FVector SpawnNiagaraOffset = FVector::ZeroVector;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Niagara|Idle")
  FVector IdleNiagaraOffset = FVector::ZeroVector;

private:
  void SpawnNiagaraEffects();

  AAuraPlayerState* GetAuraPlayerState();
  TWeakObjectPtr<AAuraPlayerState> AuraPlayerState = nullptr;
};
