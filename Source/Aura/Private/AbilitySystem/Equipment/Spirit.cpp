// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Spirit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Equipment/Rune.h"
#include "Managers/AbilityManager.h"
#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void USpirit::Spawn(UObject* WorldContextObject)
{
  Super::Spawn(WorldContextObject);

  const UAbilityInfo* AbilityData = UAuraSystemsLibrary::GetAbilitiesInfo(WorldContextObject);
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

bool USpirit::Equip(UObject* Object, int32 Slot, bool bForcesUnequip)
{
  if (!Super::Equip(Object, Slot, bForcesUnequip)) return false;

  AActor* Actor = Cast<AActor>(Object);
  if (!Actor) return false;

  UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)
    );
  if (!AuraASC) return false;

  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  FGameplayTag InputTag;
  switch (Slot)
  {
    case 0:
      {
        InputTag = AuraTags.InputTag_1;
        break;
      }
    case 1:
      {
        InputTag = AuraTags.InputTag_2;
        break;
      }
    case 2:
      {
        InputTag = AuraTags.InputTag_3;
        break;
      }
    case 3:
      {
        InputTag = AuraTags.InputTag_4;
        break;
      }
    default:
      {
        break;
      }
  }

  UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(this);
  if (!AbilityManager) return false;

  const UAbilityInfo* AbilitiesDataAsset = UAuraSystemsLibrary::GetAbilitiesInfo(this);
  const FAuraAbilityInfo& AbilityInfo = AbilitiesDataAsset->FindAbilityInfoByTag(AbilityTag);
  if (!AbilityInfo.IsValid()) return false;

  AbilityManager->GiveAbility(
    AuraASC,
    AbilityInfo,
    Level,
    InputTag
  );
  
  return true;
}

void USpirit::Unequip(UObject* Object)
{
  Super::Unequip(Object);

  AActor* Actor = Cast<AActor>(Object);
  if (!Actor) return;

  UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)
    );
  if (!AuraASC) return;
  
  UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(this);
  if (!AbilityManager) return;


  AbilityManager->RemoveAbility(AuraASC, AbilityTag);
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
