// Copyright Lucas Rossi


#include "Managers/MatchManager.h"

#include "Player/AuraPlayerState.h"
#include "Utils/AuraSystemsLibrary.h"

void UMatchManager::RegisterKills(int32 KillAmount)
{
  Scores.Kills += KillAmount;
}

void UMatchManager::RegisterDamageDone(float Damage)
{
  Scores.DamageDone += Damage;
}

void UMatchManager::RegisterDamageTaken(float Damage)
{
  Scores.DamageTaken += Damage;
}

void UMatchManager::RegisterDamagePrevented(float Damage)
{
  Scores.DamagePrevented += Damage;
}

void UMatchManager::RegisterSuccessfulDodges(int32 Amount)
{
  Scores.SuccessfulDodges += Amount;
}

void UMatchManager::RegisterManaSpent(float Mana)
{
  Scores.ManaSpent = Mana;
}

void UMatchManager::RegisterStatusEffectsApplied(int32 Amount)
{
  Scores.StatusEffectsApplied += Amount;
}

void UMatchManager::RegisterXP(int32 XP)
{
  Scores.XP += XP;
}

void UMatchManager::RegisterLoot(UEquipment* Equipment)
{
  Loot.Add(Equipment);
}

void UMatchManager::EndMatch(bool bVictory)
{
  if (bVictory)
  {
    for (const auto Equipment : Loot)
    {
      AuraPlayerState->AddEquipmentToInventory(Equipment);
    }
  }
  else
  {
    Scores.XP *= 0.5;
  }

  if (GetAuraPlayerState())
  {
    AuraPlayerState->AddXP(Scores.XP);
  }
  
  UAuraSystemsLibrary::SaveCurrentGame(this);
  UAuraSystemsLibrary::BackToHome(this);
}

AAuraPlayerState* UMatchManager::GetAuraPlayerState()
{
  if (!AuraPlayerState.IsValid())
  {
    AuraPlayerState = UAuraSystemsLibrary::GetAuraPlayerState(this);
  }

  return AuraPlayerState.Get();
}
