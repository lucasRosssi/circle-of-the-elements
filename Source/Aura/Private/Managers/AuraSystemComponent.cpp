// Copyright Lucas Rossi


#include "Managers/AuraSystemComponent.h"

#include "Utils/AuraSystemsLibrary.h"

UAuraSystemComponent::UAuraSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

UAuraSaveGame* UAuraSystemComponent::GetSaveGame()
{
  if (SaveGame == nullptr)
  {
    SaveGame = UAuraSystemsLibrary::GetCurrentSaveGameObject(this);
  }

  return SaveGame;
}
