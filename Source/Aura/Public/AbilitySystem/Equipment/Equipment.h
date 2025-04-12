// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Equipment.generated.h"

class UBaseAbility;
/**
 * 
 */
UCLASS()
class AURA_API UEquipment : public UObject
{
	GENERATED_BODY()

public:
  virtual void Spawn();
  
  virtual void Equip(UObject* Object, bool bForcesUnequip = false);
  virtual void Unequip(UObject* Object);

  FGuid GetID() const { return ID; }
  void SetID(const FGuid InID) { ID = InID; }
  FText GetEquipmentName() const { return EquipmentName; }
  void SetEquipmentName(const FText& InName) { EquipmentName = InName; }
  int32 GetLevel() const { return Level; }
  void SetLevel(int32 InLevel) { Level = InLevel; }
  
protected:
  FGuid ID;
  
  FText EquipmentName = FText();
  
  int32 Level = 1;
  
  TWeakObjectPtr<UObject> Owner = nullptr;
private:
};
