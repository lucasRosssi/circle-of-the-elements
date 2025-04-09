// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Spirit.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Equipment/Rune.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void USpirit::Spawn()
{
  Super::Spawn();

  const UAbilityInfo* AbilityData = UAuraSystemsLibrary::GetAbilitiesInfo(this);
  const FAuraAbilityInfo& AbilityInfo = AbilityData->FindAbilityInfoByTag(AbilityTag);
  EquipmentName = AbilityInfo.Name;
  
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  const int32 RandomModifierIndex = FMath::RandRange(0, AuraTags.ParentsToChildren[AuraTags.Modifiers].Num() - 1);
  ModifierTag = AuraTags.ParentsToChildren[AuraTags.Modifiers][RandomModifierIndex];
}

void USpirit::Load(
  const FGuid& InID,
  const FText& InEquipmentName,
  int32 InLevel,
  const FGameplayTag& InAbilityTag,
  const FGameplayTag& InModiferTag,
  int32 InSlots,
  const TArray<FGuid>& InRunesIDs
)
{
  ID = InID;
  EquipmentName = InEquipmentName;
  Level = InLevel;
  Slots = InSlots;
  AbilityTag = InAbilityTag;
  ModifierTag = InModiferTag;

  const AAuraPlayerState* AuraPS = UAuraSystemsLibrary::GetAuraPlayerState(this);

  if (!AuraPS) return;

  for (const FGuid& RuneID : InRunesIDs)
  {
    for (URune* Rune : AuraPS->GetRunesInventory())
    {
      if (Rune->GetID() == RuneID)
      {
        Runes.Add(Rune);
      }
    }
  }
}

void USpirit::Equip()
{
  Super::Equip();
  
}

void USpirit::Unequip()
{
  Super::Unequip();
  
}

FSpiritInfo USpirit::MakeSpiritInfo()
{
  FSpiritInfo SpiritInfo;
  SpiritInfo.ID = ID;
  SpiritInfo.EquipmentName = EquipmentName;
  SpiritInfo.Level = Level;
  SpiritInfo.Slots = Slots;
  SpiritInfo.AbilityTag = AbilityTag;
  SpiritInfo.ModifierTag = ModifierTag;
  for (const auto Rune : Runes)
  {
    SpiritInfo.Runes.Add(Rune->GetID());
  }

  return SpiritInfo;
}
