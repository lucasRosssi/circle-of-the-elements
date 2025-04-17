// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Rune.h"

void URune::Spawn(UObject* WorldContextObject)
{
  Super::Spawn(WorldContextObject);
}

void URune::Load(const FRuneInfo& RuneInfo)
{
  ID = RuneInfo.ID;
  EquipmentName = RuneInfo.EquipmentName;
  Level = RuneInfo.Level;
  RuneTag = RuneInfo.RuneTag;
}

bool URune::Equip(int32 Slot)
{
  return Super::Equip(Slot);
}

void URune::Unequip()
{
  Super::Unequip();
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
