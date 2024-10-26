// Copyright Lucas Rossi


#include "UI/WidgetController/MainMenuWidgetController.h"

#include "Aura/Aura.h"
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

void UMainMenuWidgetController::NewGameStart(const FSaveInfo& SaveData)
{
  UAuraSystemsLibrary::SaveGameData(this, SaveData);
}

void UMainMenuWidgetController::DeleteSlotConfirmed(int32 SlotIndex)
{
  UAuraSystemsLibrary::DeleteGameData(this, SlotIndex);
}

void UMainMenuWidgetController::LoadSlotAndPlay(int32 SlotIndex)
{
  UAuraSystemsLibrary::LoadGameAndPlay(this, SlotIndex);
}

UAuraSaveGame* UMainMenuWidgetController::GetSaveSlotData(int32 SlotIndex)
{
  return SaveSlots[SlotIndex];
}

void UMainMenuWidgetController::GetSelectedSaveData(FString& OutPlayerName, int32& OutSlotIndex)
{
  OutPlayerName = SelectedPlayerName;
  OutSlotIndex = SelectedSlotIndex;
}

void UMainMenuWidgetController::SetSelectedSaveData(const FString& InPlayerName, int32 InSlotIndex)
{
  SelectedPlayerName = InPlayerName;
  SelectedSlotIndex = InSlotIndex;
}
