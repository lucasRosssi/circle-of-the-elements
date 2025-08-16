// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InteractInterface.h"
#include "Gate.generated.h"

class UNiagaraComponent;
class UBoxComponent;
enum class ECardinalDirection : uint8;
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

  bool IsActive() const { return bActive; }

  UFUNCTION()
  void OnCombatFinished();
  void Enable();

  void SetIsActive(bool bIsActive);
  UFUNCTION(BlueprintNativeEvent)
  void OnActiveToggle(bool bIsActive);
  ECardinalDirection GetDirection() const { return Direction; }

protected:
  virtual void BeginPlay() override;
  
  UFUNCTION(BlueprintImplementableEvent)
  void OnInteracted(const AController* Controller);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GateMesh;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UBoxComponent> BlockerComponent;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> BarrierMesh;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UNiagaraComponent> BarrierNiagara;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UBoxComponent> InteractBox;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Location|Gate")
	ECardinalDirection Direction;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Location|Gate")
  bool bActive = false;
private:
  ULocationManager* GetLocationManager();
  TWeakObjectPtr<ULocationManager> LocationManager;
};
