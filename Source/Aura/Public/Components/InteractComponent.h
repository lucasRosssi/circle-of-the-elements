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

UENUM(BlueprintType)
enum class EInteractMode : uint8
{
  Input UMETA(DisplayName = "Input"),
  Overlap UMETA(DisplayName = "Overlap"),

  MAX UMETA(Hidden)
};

UCLASS(ClassGroup="Interaction", meta=(BlueprintSpawnableComponent))
class AURA_API UInteractComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  UInteractComponent();

  bool IsEnabled() const { return bInteractionEnabled; }
  FText GetInteractText() const { return InteractText; }
  TSubclassOf<UInteractionAbility> GetInteractAbility() const { return InteractAbility; }
  TSubclassOf<UGameplayEffect> GetInteractEffect() const { return InteractEffect; }
  
  void SetInteractAreaComponent(UPrimitiveComponent* InComponent);
  
  UFUNCTION(BlueprintCallable)
  void EnableInteraction();
  UFUNCTION(BlueprintCallable)
  void DisableInteraction();
  
  void BeginInteract(const AController* InstigatorController);
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  bool bDisableAfterInteraction = true;

protected:
  virtual void BeginPlay() override;

  UPROPERTY()
  TObjectPtr<UPrimitiveComponent> InteractAreaComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  EInteractMode InteractMode = EInteractMode::Input;

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
