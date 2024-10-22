// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractComponent.generated.h"

class UInteractionAbility;
class UGameplayEffect;
class UNiagaraSystem;
class UNiagaraComponent;
enum class ECharacterName : uint8;
class USphereComponent;

UCLASS()
class AURA_API UInteractComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UInteractComponent();

  bool IsEnabled() const { return bInteractionEnabled; }
  FText GetInteractText() const { return InteractText; }
  TSubclassOf<UInteractionAbility> GetInteractAbility() const { return InteractAbility; }
  TSubclassOf<UGameplayEffect> GetInteractEffect() const { return InteractEffect; }
  
  void SetupInteractAreaAttachment(USceneComponent* Component);

  float GetInteractAreaRadius() const;

  void EnableInteraction();
  void DisableInteraction();
  void BeginInteract(const AController* InstigatorController) const;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  bool bDisableAfterInteraction = true;

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
  TObjectPtr<USphereComponent> InteractArea;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TSubclassOf<UInteractionAbility> InteractAbility;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TSubclassOf<UGameplayEffect> InteractEffect;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  bool bInteractionEnabled = false;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  FText InteractText;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TMap<ECharacterName, TObjectPtr<UAnimMontage>> InteractMontages;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TObjectPtr<USoundBase> InteractSound;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TObjectPtr<UNiagaraSystem> InteractNiagaraSystem;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  FVector InteractNiagaraOffset = FVector::ZeroVector;

private:
  UFUNCTION()
  void OnInteractAreaOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
  );

  UFUNCTION()
  void OnInteractAreaEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
  );
};
