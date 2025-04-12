// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Rune.h"

void URune::Spawn()
{
  Super::Spawn();
}

void URune::Load(
  const FGuid& InID,
  const FText& InEquipmentName,
  int32 InLevel,
  const FGameplayTag& InRuneTag
)
{
  ID = InID;
  EquipmentName = InEquipmentName;
  Level = InLevel;
  RuneTag = InRuneTag;
}

void URune::Equip(UObject* Object, bool bForcesUnequip)
{
  Super::Equip(Object, bForcesUnequip);
}

void URune::Unequip(UObject* Object)
{
  Super::Unequip(Object);
}

FRuneInfo URune::MakeRuneInfo()
{
  FRuneInfo RuneInfo;
  RuneInfo.ID = ID;
  RuneInfo.EquipmentName = EquipmentName;
  RuneInfo.Level = Level;
  RuneInfo.RuneTag = RuneTag;
  
  return RuneInfo;
}
