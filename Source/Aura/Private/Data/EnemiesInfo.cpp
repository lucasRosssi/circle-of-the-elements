// Copyright Lucas Rossi


#include "Data/EnemiesInfo.h"

#include "Aura/AuraMacros.h"

FEnemyInfo UEnemiesInfo::GetEnemyInfo(const FGameplayTag& EnemyTag) const
{
  const FEnemyInfo* EnemyInfo = Enemies.Find(EnemyTag);

  GUARD(EnemyInfo != nullptr, FEnemyInfo(), TEXT("Enemy info not found: %s"), *EnemyTag.ToString())
  
  return *Enemies.Find(EnemyTag);
}
