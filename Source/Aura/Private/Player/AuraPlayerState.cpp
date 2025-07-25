// Copyright Lucas Rossi


#include "Player/AuraPlayerState.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/Equipment/Rune.h"
#include "AbilitySystem/Equipment/Spirit.h"
#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Character/AuraCharacterBase.h"
#include "Game/AuraSaveGame.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "UI/WidgetController/LoadoutWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraPlayerState::AAuraPlayerState()
{
  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
  AbilitySystemComponent->SetIsReplicated(true);
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

  AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

  SetNetUpdateFrequency(100.f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(AAuraPlayerState, Level);
  DOREPLIFETIME(AAuraPlayerState, XP);
  DOREPLIFETIME(AAuraPlayerState, AttributePoints);
  DOREPLIFETIME(AAuraPlayerState, SkillPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
  return AbilitySystemComponent;
}

UAuraAbilitySystemComponent* AAuraPlayerState::GetAuraASC()
{
  if (AuraASC == nullptr)
  {
    AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
  }

  return AuraASC;
}

float AAuraPlayerState::GetAttackSpeed_Implementation()
{
  return AttributeSet->GetAttackSpeed();
}

float AAuraPlayerState::GetCastSpeed_Implementation()
{
  return AttributeSet->GetCastSpeed();
}

float AAuraPlayerState::GetMovementSpeed_Implementation()
{
  return AttributeSet->GetMovementSpeed();
}

void AAuraPlayerState::SetMovementSpeed_Implementation(float InMovementSpeed)
{
  if (GetCharacterBase())
  {
    CharacterBase->ChangeMovementSpeed(InMovementSpeed);
  }
}

float AAuraPlayerState::GetPower_Implementation()
{
  return AttributeSet->GetPower();
}

void AAuraPlayerState::SetTimeDilation_Implementation(float InTimeDilation)
{
  GetPawn()->CustomTimeDilation = InTimeDilation;
}

bool AAuraPlayerState::Equip_Implementation(const FGuid& ID, int32 Slot)
{
  if (!ID.IsValid()) return false;
  
  const USpirit* FoundSpirit = FindSpirit(ID);

  if (FoundSpirit == nullptr) return false;

  if (Loadout.Contains(Slot) && Loadout[Slot].IsValid())
  {
    USpirit* SlotSpirit = FindSpirit(Loadout[Slot]);

    if (SlotSpirit)
    {
      SlotSpirit->Unequip();
    }
  }
  
  Loadout.Add(Slot, ID);

  if (GetSaveGame() && bInitialized)
  {
    SaveGame->PlayerState.Loadout = Loadout;
  }

  return true;
}

void AAuraPlayerState::Unequip_Implementation(const FGuid& ID)
{
  if (!ID.IsValid()) return;

  const int32* Slot = Loadout.FindKey(ID);

  if (Slot && *Slot >= 0)
  {
    const int32 SlotNum = *Slot;
    Loadout.Remove(SlotNum);

    const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
    FAuraAbilityInfo AbilityInfo;
    AbilityInfo.AbilityTag = AuraTags.Abilities_NONE;
    AbilityInfo.InputTag = AuraTags.ParentsToChildren[AuraTags.InputTag][SlotNum];
    UAuraAbilitySystemLibrary::GetOverlayWidgetController(this)
      ->AbilityInfoDelegate.Broadcast(AbilityInfo);
    
    if (GetSaveGame())
    {
      SaveGame->PlayerState.Loadout = Loadout;
    }
  }
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
  Level = FMath::Clamp(InLevel, 1, MAX_LEVEL);
  OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddLevel(int32 InLevel)
{
  if (InLevel < 1 || Level + InLevel > MAX_LEVEL) return;

  const int32 OldLevel = Level;
  Level += InLevel;
  IPlayerInterface::Execute_LevelUp(AbilitySystemComponent->GetAvatarActor());
  OnLevelChangedDelegate.Broadcast(Level);

  const int32 APReward = LevelInfo.Get()->GetLevelUpAttributePoints(OldLevel, Level);
  const int32 SPReward = LevelInfo.Get()->GetLevelUpSkillPoints(OldLevel, Level);

  AddAttributePoints(APReward);
  AddSkillPoints(SPReward);
}

void AAuraPlayerState::SetXP(int32 InXP)
{
  if (XP == InXP) return;

  XP = InXP;
  if (GetSaveGame())
  {
    SaveGame->PlayerState.XP = XP;
  }
  OnXPChangedDelegate.Broadcast(XP);

  const int32 XPLevel = LevelInfo.Get()->FindLevelByXP(XP);
  SetLevel(XPLevel);
}

void AAuraPlayerState::AddXP(int32 InXP)
{
  if (InXP < 1) return;

  XP += InXP;
  if (GetSaveGame())
  {
    SaveGame->PlayerState.XP = XP;
  }
  OnXPChangedDelegate.Broadcast(XP);
  
  const int32 LevelUpAmount = LevelInfo.Get()->FindLevelByXP(XP) - Level;
  if (LevelUpAmount > 0)
  {
    AddLevel(LevelUpAmount);
  }
}

void AAuraPlayerState::SetAttributePoints(int32 InAttributePoints)
{
  if (AttributePoints == InAttributePoints) return;

  AttributePoints = InAttributePoints;
  OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddAttributePoints(int32 InAttributePoints)
{
  if (InAttributePoints == 0) return;

  AttributePoints += InAttributePoints;

  if (GetSaveGame())
  {
    SaveGame->PlayerState.AttributePoints = AttributePoints;
  }
  
  OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSkillPoints(int32 InSkillPoints)
{
  if (SkillPoints == InSkillPoints) return;

  SkillPoints = InSkillPoints;
  OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::AddSkillPoints(int32 InSkillPoints)
{
  if (InSkillPoints == 0) return;

  SkillPoints += InSkillPoints;

  if (GetSaveGame())
  {
    SaveGame->PlayerState.SkillPoints = SkillPoints;
  }
  
  OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::AddEquipmentToInventory(UEquipment* InEquipment)
{
  InEquipment->SetOwner(this);
  
  if (USpirit* Spirit = Cast<USpirit>(InEquipment))
  {
    SpiritsInventory.Add(Spirit);

    if (GetSaveGame())
    {
      const FSpiritInfo& SpiritInfo = Spirit->MakeSpiritInfo();
      SaveGame->PlayerState.SpiritsInventory.Add(SpiritInfo);
    }
  }
  else if (URune* Rune = Cast<URune>(InEquipment))
  {
    RunesInventory.Add(Rune);

    if (GetSaveGame())
    {
      const FRuneInfo& RuneInfo = Rune->MakeRuneInfo();
      SaveGame->PlayerState.RunesInventory.Add(RuneInfo);
    }
  }
}

void AAuraPlayerState::RemoveEquipmentFromInventory(UEquipment* InEquipment)
{
  if (USpirit* Spirit = Cast<USpirit>(InEquipment))
  {
    SpiritsInventory.Remove(Spirit);

    UAuraAbilitySystemLibrary::GetLoadoutWidgetController(this)
      ->OnRemovedDelegate.Broadcast(Spirit);

    if (GetSaveGame())
    {
      int32 SpiritToRemoveIndex = -1;
      for (int32 i = 0; i < SaveGame->PlayerState.SpiritsInventory.Num(); i++)
      {
        if (SaveGame->PlayerState.SpiritsInventory[i].ID == Spirit->GetID())
        {
          SpiritToRemoveIndex = i;
          break;
        }
      }

      if (SpiritToRemoveIndex >= 0)
      {
        SaveGame->PlayerState.SpiritsInventory.RemoveAt(SpiritToRemoveIndex);
      }
    }
  }
  else if (URune* Rune = Cast<URune>(InEquipment))
  {
    // RunesInventory.Add(Rune);
    //
    // if (GetSaveGame())
    // {
    //   const FRuneInfo& RuneInfo = Rune->MakeRuneInfo();
    //   SaveGame->PlayerState.RunesInventory.Add(RuneInfo);
    // }
  }
}

void AAuraPlayerState::AddPlayerResource(const FGameplayTag& ResourceTag, int32 Amount)
{
  if (int32* CurrentAmount = Resources.Find(ResourceTag))
  {
    *CurrentAmount += Amount;
    if (*CurrentAmount < 0) *CurrentAmount = 0;

    if (GetSaveGame())
    {
      if (int32* SaveAmount = SaveGame->PlayerState.Resources.Find(ResourceTag))
      {
        *SaveAmount = *CurrentAmount;
      }
      else
      {
        SaveGame->PlayerState.Resources.Add(ResourceTag, *CurrentAmount);
      }
    }
    
    OnResourceChangedDelegate.Broadcast(ResourceTag, Amount);
  }
  else
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Could not find %s resource in the player state!"),
      *ResourceTag.ToString()
    );
  }
}

int32 AAuraPlayerState::GetPlayerResourceByTag(const FGameplayTag& ResourceTag)
{
  const int32* Amount = Resources.Find(ResourceTag);
  
  return Amount ? *Amount : 0;
}

bool AAuraPlayerState::CanAffordResourceCost(const TMap<FGameplayTag, int32>& CostMap) const
{
  for (const auto& [Resource, Cost] : CostMap)
  {
    const int32* PlayerAmount = Resources.Find(Resource);

    if (!PlayerAmount)
    {
      UE_LOG(
        LogAura,
        Error,
        TEXT("Could not find %s cost resource in the player state!"),
        *Resource.ToString()
        );
      continue;
    }

    if (*PlayerAmount < Cost) return false;
  }
  return true;
}

void AAuraPlayerState::InitializeState()
{
  if (!bOverridePlayerLevel && GetSaveGame())
  {
    SetXP(SaveGame->PlayerState.XP);
    SetSkillPoints(SaveGame->PlayerState.SkillPoints);
    SetAttributePoints(SaveGame->PlayerState.AttributePoints);
    
    for (auto& [Resource, SaveAmount] : SaveGame->PlayerState.Resources)
    {
      if (int32* PlayerAmount = Resources.Find(Resource))
      {
        *PlayerAmount = SaveAmount;
      }
    }

    for (const FRuneInfo& RuneInfo : SaveGame->PlayerState.RunesInventory)
    {
      URune* Rune = NewObject<URune>();
      Rune->Load(RuneInfo);
      Rune->SetOwner(this);
      RunesInventory.Add(Rune);
    }
    
    for (const FSpiritInfo& SpiritInfo : SaveGame->PlayerState.SpiritsInventory)
    {
      USpirit* Spirit = NewObject<USpirit>();
      Spirit->SetOwner(this);
      Spirit->Load(SpiritInfo);
      SpiritsInventory.Add(Spirit);
    }

    for (const auto& [Slot, SpiritID] : SaveGame->PlayerState.Loadout)
    {
      if (USpirit* Spirit = FindSpirit(SpiritID))
      {
        Spirit->Equip(Slot);
      }
    }
  }

  bInitialized = true;
}

AAuraCharacterBase* AAuraPlayerState::GetCharacterBase()
{
  if (CharacterBase == nullptr)
  {
    CharacterBase = Cast<AAuraCharacterBase>(GetPawn());
  }

  return CharacterBase;
}

UAuraSaveGame* AAuraPlayerState::GetSaveGame()
{
  if (SaveGame == nullptr)
  {
    SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(this);
  }

  return SaveGame;
}

USpirit* AAuraPlayerState::FindSpirit(const FGuid& ID)
{
  USpirit** FoundSpirit = SpiritsInventory.FindByPredicate(
    [ID](const USpirit* Spirit) { return Spirit->GetID() == ID; }
    );

  if (!FoundSpirit) return nullptr;

  return *FoundSpirit;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
  OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
  OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
  OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
  OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}
