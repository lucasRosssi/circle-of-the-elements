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

bool UEquipment::Equip(UObject* Object, int32 Slot, bool bForcesUnequip)
{
  if (!IsValid(Object) || !Object->Implements<UEquipperInterface>()) return false;
  
  if (Owner.IsValid())
  {
    if (bForcesUnequip)
    {
      Unequip(Owner.Get());
    }
    else
    {
      UE_LOG(
        LogAura,
        Error,
        TEXT("[Equipment] %s is already equipped by %s"),
        *GetName(),
        *Owner.Get()->GetName()
        );
      return false;
    }
  }

  const bool bEquipped = IEquipperInterface::Execute_Equip(Object, ID, Slot);

  return bEquipped;
}

void UEquipment::Unequip(UObject* Object)
{
  Owner = nullptr;
}
