// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Equipment.h"

#include "Aura/Aura.h"
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

void UEquipment::Equip()
{
}

void UEquipment::Unequip()
{
}
