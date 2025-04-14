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
  virtual void Spawn(UObject* WorldContextObject) override;
  void Load(const FRuneInfo& RuneInfo);

  virtual bool Equip(UObject* Object, int32 Slot, bool bForcesUnequip = false) override;
  virtual void Unequip(UObject* Object) override;

  FRuneInfo MakeRuneInfo();
  
protected:
  FGameplayTag RuneTag;

  
private:
  
};
