// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SpiritActor.generated.h"

class USphereComponent;
class UGameplayAbility;
class AAuraHero;
class UBaseAbility;
class UNiagaraComponent;
class UAbilitySystemComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHijackerSet, AActor*, Hijacker);

UCLASS()
class AURA_API ASpiritActor : public AActor
{
  GENERATED_BODY()

public:
  ASpiritActor();

  void SetAbilityTag(const FGameplayTag& InTag);
  void SetElementTag(const FGameplayTag& InTag);
  UFUNCTION(BlueprintPure)
  FGameplayTag GetElementTag() const { return ElementTag; }

  void SetCooldownTag(const FGameplayTag& InTag);
  void SetChargeTagAndCount(const FGameplayTag& InTag, int32 ChargesCount);

  UFUNCTION(BlueprintCallable)
  void SetHijacker(AActor* InHijacker);
  AActor* GetHijacker() const { return Hijacker; }
  UFUNCTION(BlueprintPure)
  bool IsHijacked() const { return Hijacker != nullptr; }

  UFUNCTION(BlueprintNativeEvent)
  UNiagaraComponent* GetSpiritNiagara();

  UFUNCTION(BlueprintPure)
  bool IsUsingAbility() const { return bUsingAbility; }

  FVector GetAbilityUseLocation();
  FVector GetHijackLocation();

  FOnHijackerSet OnHijackerSetDelegate;

protected:
  virtual void BeginPlay() override;

  void ListenForCooldownChange();
  void ListenForAbilityActivation();

  UFUNCTION()
  void OnHijackerDeath(AActor* DeadHijacker);
  UFUNCTION()
  void OnHijackerOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
  );

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<USphereComponent> SpiritCollision;

  UPROPERTY(EditDefaultsOnly, Category="Spirit")
  TArray<FName> NiagaraScaleParameters = TArray({FName("Scale")});

private:
  void AbilityActivated(UGameplayAbility* InAbility);

  void AbilityStateChanged(bool bActivated);
  void CooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
  void ChargeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

  UAbilitySystemComponent* GetOwnerASC();

  FGameplayTag AbilityTag;
  FGameplayTag ElementTag;
  FGameplayTag CooldownTag;
  FGameplayTag ChargeTag;
  int32 MaxChargesCount = 0;
  bool bUsingAbility = false;

  FDelegateHandle OnAnyAbilityActivated;

  UPROPERTY()
  UBaseAbility* Ability = nullptr;
  UPROPERTY()
  UAbilitySystemComponent* OwnerASC = nullptr;
  UPROPERTY()
  AActor* Hijacker = nullptr;
};
