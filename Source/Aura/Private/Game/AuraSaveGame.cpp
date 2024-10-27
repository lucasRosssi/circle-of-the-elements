// Copyright Lucas Rossi


#include "Game/AuraSaveGame.h"

#include "AuraGameplayTags.h"

UAuraSaveGame::UAuraSaveGame()
{
  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  if (PlayerState.Resources.IsEmpty())
  {
    for (const auto& Tag : AuraTags.ParentsToChildren[AuraTags.Resources_Essence])
    {
      PlayerState.Resources.Add(Tag, 0);
    }
  }

  if (AttributeSet.Attributes.IsEmpty())
  {
    for (const auto& Tag : AuraTags.ParentsToChildren[AuraTags.Attributes_Primary])
    {
      AttributeSet.Attributes.Add(Tag, 10);
    }
  }
}
