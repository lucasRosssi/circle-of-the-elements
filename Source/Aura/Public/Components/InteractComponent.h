// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractComponent.generated.h"

class AAuraPlayerController;
class UNiagaraSystem;
class UNiagaraComponent;
enum class ECharacterName : uint8;
class AAuraGameModeBase;
class USphereComponent;

DECLARE_DELEGATE_OneParam(FOnInteracted, const AAuraPlayerController* /* InstigatorController */)
DECLARE_DELEGATE_OneParam(FOnPostInteracted, const AAuraPlayerController* /* InstigatorController */)

UCLASS()
class AURA_API UInteractComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UInteractComponent();

  void SetupInteractAreaAttachment(USceneComponent* Component);

  float GetInteractAreaRadius();

  UFUNCTION()
  void EnableInteraction();
  UFUNCTION()
  void DisableInteraction();

  FOnInteracted OnInteractedDelegate;
  FOnPostInteracted OnPostInteractedDelegate;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  bool bDisableAfterInteraction = true;

protected:
  virtual void BeginPlay() override;

  TWeakObjectPtr<AAuraGameModeBase> AuraGameMode = nullptr;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
  TObjectPtr<USphereComponent> InteractArea;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  bool bInteractionEnabled = false;

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
  void PreInteract(const AAuraPlayerController* InstigatorController);
  void Interact(const AAuraPlayerController* InstigatorController);
  void PostInteract(const AAuraPlayerController* InstigatorController);

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
