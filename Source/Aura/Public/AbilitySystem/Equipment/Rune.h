// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Equipment/Equipment.h"
#include "Rune.generated.h"

USTRUCT(BlueprintType)
struct FRuneInfo
{
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  FGuid ID;
  UPROPERTY(BlueprintReadWrite)
  FText EquipmentName = FText();
  UPROPERTY(BlueprintReadWrite)
  int32 Level = 1;
  UPROPERTY(BlueprintReadWrite)
  FGameplayTag RuneTag;
};

/**
 * 
 */
UCLASS()
class AURA_API URune : public UEquipment
{
	GENERATED_BODY()

public:
  virtual void Spawn() override;
  void Load(
    const FGuid& InID,
    const FText& InEquipmentName,
    int32 InLevel,
    const FGameplayTag& InRuneTag
  );

  virtual void Equip(UObject* Object, bool bForcesUnequip = false) override;
  virtual void Unequip(UObject* Object) override;

  FRuneInfo MakeRuneInfo();
  
protected:
  FGameplayTag RuneTag;

  
private:
  
};
