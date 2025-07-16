// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Equipment/Equipment.h"
#include "Interfaces/EquipperInterface.h"
#include "Spirit.generated.h"

class ASpiritActor;
class URune;

USTRUCT(BlueprintType)
struct FSpiritInfo
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FGuid ID;
  UPROPERTY(BlueprintReadWrite)
  FText EquipmentName = FText();
  UPROPERTY(BlueprintReadWrite)
  int32 Level = 1;
  UPROPERTY(BlueprintReadWrite)
  FGameplayTag AbilityTag;
  UPROPERTY(BlueprintReadWrite)
  FGameplayTag ElementTag;
  UPROPERTY(BlueprintReadWrite)
  FGameplayTag ModifierTag;
  UPROPERTY(BlueprintReadWrite)
  int32 MySlot = -1;
  UPROPERTY(BlueprintReadWrite)
  int32 RuneSlots = 1;
  UPROPERTY(BlueprintReadWrite)
  TArray<FGuid> Runes;
};

/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API USpirit : public UEquipment, public IEquipperInterface
{
	GENERATED_BODY()

public:
  virtual void Spawn(UObject* WorldContextObject) override;
  void Load(const FSpiritInfo& SpiritInfo);

  virtual bool Equip(int32 Slot) override;
  virtual void Unequip() override;
  virtual FString GetEquipmentDescription(bool bNextLevel) override;

  UFUNCTION(BlueprintPure)
  int32 GetSlot() const { return MySlot; }

  UFUNCTION(BlueprintCallable)
  FSpiritInfo MakeSpiritInfo();

  void SetAbilityTag(const FGameplayTag& InAbilityTag) { AbilityTag = InAbilityTag; }
  void SetElementTag(const FGameplayTag& InElementTag) { ElementTag = InElementTag; }
  FGameplayTag GetAbilityTag() { return AbilityTag; }

  UFUNCTION(BlueprintPure)
  ASpiritActor* GetSpiritActor() const { return SpiritActor; }

protected:
  UPROPERTY(BlueprintReadOnly)
  FGameplayTag AbilityTag;
  UPROPERTY(BlueprintReadOnly)
  FGameplayTag ElementTag;

  FGameplayTag ModifierTag;

  int32 MySlot = -1;

  int32 RuneSlots = 1;

  UPROPERTY(  )
  TArray<URune*> Runes;

private:
  UPROPERTY()
  TObjectPtr<ASpiritActor> SpiritActor = nullptr;
};
