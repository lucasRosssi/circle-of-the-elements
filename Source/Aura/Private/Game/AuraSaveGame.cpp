// Copyright Lucas Rossi


#include "Game/AuraSaveGame.h"

#include "AuraGameplayTags.h"

UAuraSaveGame::UAuraSaveGame()
{
  InitPlayerState();
  InitAttributeSet();
}

void UAuraSaveGame::PostInitProperties()
{
  Super::PostInitProperties();

  bJustLoaded = true;
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
