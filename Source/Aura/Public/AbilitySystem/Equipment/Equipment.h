// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Equipment.generated.h"

class UAuraSaveGame;
class UBaseAbility;
/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API UEquipment : public UObject
{
	GENERATED_BODY()

public:
  virtual void Spawn(UObject* WorldContextObject);
  
  virtual bool Equip(UObject* Object, int32 Slot, bool bForcesUnequip = false);
  virtual void Unequip(UObject* Object);

  virtual FString GetEquipmentDescription();
  
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
