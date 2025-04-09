// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Equipment/Equipment.h"
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
class AURA_API USpirit : public UEquipment
{
	GENERATED_BODY()

public:
  virtual void Spawn() override;
  void Load(
    const FGuid& InID,
    const FText& InEquipmentName,
    int32 InLevel,
    const FGameplayTag& InAbilityTag,
    const FGameplayTag& InModiferTag,
    int32 InSlots,
    const TArray<FGuid>& InRunesIDs
  );

  virtual void Equip() override;
  virtual void Unequip() override;

  FSpiritInfo MakeSpiritInfo();
  
protected:
  FGameplayTag AbilityTag;

  FGameplayTag ModifierTag;  

  int32 Slots = 1;
  
  TArray<URune*> Runes;

private:
  
};
