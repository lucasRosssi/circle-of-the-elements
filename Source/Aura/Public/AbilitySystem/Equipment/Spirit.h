// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Equipment/Equipment.h"
#include "Interfaces/EquipperInterface.h"
#include "Spirit.generated.h"

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
  FGameplayTag ModifierTag;
  UPROPERTY(BlueprintReadWrite)
  int32 Slots = 1;
  UPROPERTY(BlueprintReadWrite)
  TArray<FGuid> Runes;
};

/**
 * 
 */
UCLASS()
class AURA_API USpirit : public UEquipment, public IEquipperInterface
{
	GENERATED_BODY()

public:
  virtual void Spawn(UObject* WorldContextObject) override;
  void Load(const FSpiritInfo& SpiritInfo);

  virtual bool Equip(UObject* Object, int32 Slot, bool bForcesUnequip = false) override;
  virtual void Unequip(UObject* Object) override;

  FSpiritInfo MakeSpiritInfo();

  void SetAbilityTag(const FGameplayTag& InAbilityTag) { AbilityTag = InAbilityTag; }
  
protected:
  FGameplayTag AbilityTag;

  FGameplayTag ModifierTag;  

  int32 Slots = 1;
  
  TArray<URune*> Runes;

private:
  
};
