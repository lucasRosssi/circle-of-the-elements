// Copyright Lucas Rossi


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Character/AuraCharacterBase.h"
#include "Game/AuraSaveGame.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Utils/AuraSystemsLibrary.h"

AAuraPlayerState::AAuraPlayerState()
{
  AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
  AbilitySystemComponent->SetIsReplicated((true));
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

  AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

  NetUpdateFrequency = 100.f;
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

float AAuraPlayerState::GetActionSpeed_Implementation()
{
  return AttributeSet->GetActionSpeed();
}

void AAuraPlayerState::SetActionSpeed_Implementation(float InActionSpeed)
{
  if (GetCharacterBase())
  {
    CharacterBase->ChangeActionSpeed(InActionSpeed);
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

void AAuraPlayerState::SetLevel(int32 InLevel)
{
  Level = InLevel;
  OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddLevel(int32 InLevel)
{
  if (InLevel < 1) return;

  const int32 OldLevel = Level;
  Level += InLevel;
  IPlayerInterface::Execute_LevelUp(AbilitySystemComponent->GetAvatarActor());
  OnLevelChangedDelegate.Broadcast(Level);

  const int32 APReward = LevelInfo.Get()->GetLevelUpAttributePoints(OldLevel, Level);
  const int32 SPReward = LevelInfo.Get()->GetLevelUpSkillPoints(OldLevel, Level);

  AddAttributePoints(APReward);
  AddSkillPoints(SPReward);

  // if (GetAuraASC())
  // {
  // 	GetAuraASC()->UpdateAbilityStatuses(Level);
  // }
}

void AAuraPlayerState::SetXP(int32 InXP)
{
  if (XP == InXP) return;

  XP = InXP;
  OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddXP(int32 InXP)
{
  if (InXP < 1) return;

  XP += InXP;
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
  if (GetSaveGame())
  {
    SetSkillPoints(SaveGame->PlayerState.SkillPoints);
    SetAttributePoints(SaveGame->PlayerState.AttributePoints);
    
    for (auto& [Resource, SaveAmount] : SaveGame->PlayerState.Resources)
    {
      if (int32* PlayerAmount = Resources.Find(Resource))
      {
        *PlayerAmount = SaveAmount;
      }
    }
  }
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
