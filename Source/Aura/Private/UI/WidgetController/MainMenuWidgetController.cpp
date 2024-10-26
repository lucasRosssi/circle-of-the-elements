// Copyright Lucas Rossi


#include "UI/WidgetController/MainMenuWidgetController.h"

#include "Aura/Aura.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/AuraSystemsLibrary.h"

void UMainMenuWidgetController::InitializeSaveSlots()
{
  for (int32 i = 0; i < MAX_SAVE_SLOTS; i++)
  {
    if (UAuraSaveGame* SaveGame = UAuraSystemsLibrary::LoadGameData(this, i))
    {
      SaveSlots.Add(SaveGame);
      continue;
    }
    
    SaveSlots.Add(nullptr);
  }
}

void UMainMenuWidgetController::NewSlotPressed(const FString& EnteredName, int32 SlotIndex)
{
  UAuraSystemsLibrary::SaveGameData(this, EnteredName, SlotIndex);
}

void UMainMenuWidgetController::DeleteSlotConfirmed(int32 SlotIndex)
{
  UAuraSystemsLibrary::DeleteGameData(this, SlotIndex);
}

UAuraSaveGame* UMainMenuWidgetController::GetSaveSlotData(int32 SlotIndex)
{
  return SaveSlots[SlotIndex];
}
