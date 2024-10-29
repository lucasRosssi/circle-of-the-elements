// Copyright Lucas Rossi


#include "Game/AuraSaveGame.h"

#include "AuraGameplayTags.h"

UAuraSaveGame::UAuraSaveGame()
{
  InitPlayerState();
  InitAttributeSet();
  InitAbilityInputs();
}

void UAuraSaveGame::PostInitProperties()
{
  Super::PostInitProperties();

  bJustLoaded = true;
}

void UAuraSaveGame::OnPlayerDeath()
{
  PlayerState.Reset();
  AttributeSet.Reset();
  AbilityManager.Reset();
  AbilityInput.Reset();
  CombatManager.Reset();
  LocationManager.Reset();
  RewardManager.Reset();

  SaveInfo.RegionName = ERegion::Village;
}

void UAuraSaveGame::InitPlayerState()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  if (PlayerState.Resources.IsEmpty())
  {
    for (const auto& Tag : AuraTags.ParentsToChildren[AuraTags.Resources_Essence])
    {
      PlayerState.Resources.Add(Tag, 0);
    }
  }
}

void UAuraSaveGame::InitAttributeSet()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  
  if (AttributeSet.Attributes.IsEmpty())
  {
    for (const auto& Tag : AuraTags.ParentsToChildren[AuraTags.Attributes_Primary])
    {
      AttributeSet.Attributes.Add(Tag, 10);
    }
  }
}

void UAuraSaveGame::InitAbilityInputs()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
  
  AbilityInput.Inputs.Add(AuraTags.InputTag_1, FGameplayTag());
  AbilityInput.Inputs.Add(AuraTags.InputTag_2, FGameplayTag());
  AbilityInput.Inputs.Add(AuraTags.InputTag_3, FGameplayTag());
  AbilityInput.Inputs.Add(AuraTags.InputTag_4, FGameplayTag());
}
