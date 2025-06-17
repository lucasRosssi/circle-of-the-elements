// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Equipment.h"

#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Interfaces/EquipperInterface.h"
#include "Managers/LocationManager.h"
#include "Utils/AuraSystemsLibrary.h"

void UEquipment::Spawn(UObject* WorldContextObject)
{
  const int32 RecommendedLevel = UAuraSystemsLibrary::GetLocationManager(WorldContextObject)
    ->GetCurrentLocationRecommendedLevel();
  
  const int32 Max = FMath::Min(RecommendedLevel + 3, MAX_LEVEL);
  Level = FMath::RandRange(RecommendedLevel, Max);

  ID = FGuid::NewGuid();
}

bool UEquipment::Equip(int32 Slot)
{
  if (!Owner.IsValid() || !Owner->Implements<UEquipperInterface>())
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("[Equipment] %s doesn't belong to any Owner"),
      *GetName()
      );
    return false;
  }

  const bool bEquipped = IEquipperInterface::Execute_Equip(Owner.Get(), ID, Slot);

  return bEquipped;
}

void UEquipment::Unequip()
{
  if (!Owner.IsValid() || !Owner->Implements<UEquipperInterface>())
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("[Equipment] %s doesn't belong to any Owner"),
      *GetName()
      );
    return;
  }

  IEquipperInterface::Execute_Unequip(Owner.Get(), ID);
}

FString UEquipment::GetEquipmentDescription()
{
  return FString();
}
