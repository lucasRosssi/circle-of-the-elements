// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Equipment.h"

#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Interfaces/EquipperInterface.h"
#include "Managers/LocationManager.h"
#include "Utils/AuraSystemsLibrary.h"

void UEquipment::Spawn()
{
  const int32 RecommendedLevel = UAuraSystemsLibrary::GetLocationManager(this)
    ->GetCurrentLocationRecommendedLevel();
  
  const int32 Max = FMath::Min(RecommendedLevel + 3, MAX_LEVEL);
  Level = FMath::RandRange(RecommendedLevel, Max);

  ID = FGuid::NewGuid();
}

void UEquipment::Equip(UObject* Object, bool bForcesUnequip)
{
  if (!IsValid(Object) || !Object->Implements<UEquipperInterface>()) return;
  
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
      return;
    }
  }

  IEquipperInterface::
  
  Owner = Object;
}

void UEquipment::Unequip(UObject* Object)
{
  Owner = nullptr;
}
