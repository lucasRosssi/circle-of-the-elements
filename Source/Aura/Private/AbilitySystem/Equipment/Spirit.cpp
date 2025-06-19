// Copyright Lucas Rossi


#include "AbilitySystem/Equipment/Spirit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Equipment/Rune.h"
#include "Actor/SpiritActor.h"
#include "Aura/AuraLogChannels.h"
#include "Components/OrbitManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AbilityManager.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"

void USpirit::Spawn(UObject* WorldContextObject)
{
  Super::Spawn(WorldContextObject);

  const UAbilityInfo* AbilityData = UAuraSystemsLibrary::GetAbilitiesInfo(WorldContextObject);
  const FAuraAbilityInfo& AbilityInfo = AbilityData->FindAbilityInfoByTag(AbilityTag);
  
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  const int32 RandomModifierIndex = FMath::RandRange(0, AuraTags.ParentsToChildren[AuraTags.Modifiers].Num() - 1);
  ModifierTag = AuraTags.ParentsToChildren[AuraTags.Modifiers][RandomModifierIndex];
  
  EquipmentName = FText::FormatOrdered(
    FText::FromString("{0} - {1}"),
    AbilityInfo.Name,
    FText::FromString(ModifierTag.ToString())
  );
}

void USpirit::Load(const FSpiritInfo& SpiritInfo)
{
  ID = SpiritInfo.ID;
  EquipmentName = SpiritInfo.EquipmentName;
  Level = SpiritInfo.Level;
  Slots = SpiritInfo.Slots;
  AbilityTag = SpiritInfo.AbilityTag;
  ModifierTag = SpiritInfo.ModifierTag;

  if (!Owner.IsValid()) return;
  
  const AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(Owner.Get());

  if (!AuraPS) return;

  for (const FGuid& RuneID : SpiritInfo.Runes)
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

bool USpirit::Equip(int32 Slot)
{
  if (!Super::Equip(Slot)) return false;

  AActor* Actor = Cast<AActor>(Owner.Get());
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

  if (!InputTag.IsValid())
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Slot %d is invalid for equipping %s!"),
      Slot,
      *EquipmentName.ToString()
    );
    return false;
  }

  UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(Actor);

  const UAbilityInfo* AbilitiesDataAsset = UAuraSystemsLibrary::GetAbilitiesInfo(Actor);
  const FAuraAbilityInfo& AbilityInfo = AbilitiesDataAsset->FindAbilityInfoByTag(AbilityTag);
  if (!AbilityInfo.IsValid()) return false;

  AbilityManager->GiveAbility(
    AuraASC,
    AbilityInfo,
    Level,
    InputTag
  );

  AActor* AvatarActor = AuraASC->GetAvatarActor();
  if (IsValid(AbilityInfo.SpiritActor) && IsValid(AvatarActor))
  {
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(AvatarActor->GetActorLocation() + FVector(50.f, 50.f, 50.f) * (Slot + 1));
    
    SpiritActor = AvatarActor->GetWorld()->SpawnActorDeferred<ASpiritActor>(
        AbilityInfo.SpiritActor,
        SpawnTransform,
        AvatarActor,
        nullptr,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
      );
    
    if (SpiritActor)
    {
      SpiritActor->FinishSpawning(SpawnTransform);
      if (UOrbitManagerComponent* OrbitManager = AvatarActor->FindComponentByClass<UOrbitManagerComponent>())
      {
        OrbitManager->RegisterSpirit(SpiritActor);
      }
    }
  }
  
  return true;
}

void USpirit::Unequip()
{
  Super::Unequip();
  
  AActor* Actor = Cast<AActor>(Owner.Get());
  if (!Actor) return;

  UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor)
    );
  if (!AuraASC) return;
  
  UAbilityManager* AbilityManager = UAuraSystemsLibrary::GetAbilityManager(Actor);
  AbilityManager->RemoveAbility(AuraASC, AbilityTag);

  if (!IsValid(SpiritActor)) return;

  if (UOrbitManagerComponent* OrbitManager = AuraASC->GetAvatarActor()->FindComponentByClass<UOrbitManagerComponent>())
  {
    OrbitManager->UnregisterSpirit(SpiritActor);
  }
}

FString USpirit::GetEquipmentDescription()
{
  return UAuraAbilitySystemLibrary::GetAbilityDescription(
    UAuraSystemsLibrary::GetAbilitiesInfo(Owner.Get()),
    AbilityTag,
    Level
  );
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
