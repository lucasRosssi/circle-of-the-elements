// Copyright Lucas Rossi


#include "Game/AuraGameInstance.h"

#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Character/Data/HeroInfo.h"
#include "Game/AuraSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UAuraGameInstance::SaveGameData(const FString& InPlayerName, int32 SlotIndex)
{
  const FString SlotName = GetSlotName(SlotIndex);
  if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
  {
    UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
  }
  
  USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UAuraSaveGame::StaticClass());
  UAuraSaveGame* AuraSaveGame = Cast<UAuraSaveGame>(SaveGameObject);
  AuraSaveGame->PlayerName = InPlayerName;
  AuraSaveGame->SlotName = SlotName;
  AuraSaveGame->SlotIndex = SlotIndex;

  UGameplayStatics::SaveGameToSlot(AuraSaveGame, SlotName, SlotIndex);

  if (!SaveGame) SaveGame = AuraSaveGame;
}

UAuraSaveGame* UAuraGameInstance::LoadGameData(int32 SlotIndex)
{
  const FString SlotName = GetSlotName(SlotIndex);
  if (!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
  {
    return nullptr;
  }
  
  USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
  
  return Cast<UAuraSaveGame>(SaveGameObject);
}

void UAuraGameInstance::DeleteGameData(int32 SlotIndex)
{
  const FString SlotName = GetSlotName(SlotIndex);
  if (!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Trying to delete a nonexistent save file: %s."),
      *SlotName
    )
    return;
  }

  UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

FHeroData UAuraGameInstance::GetCurrentHeroData() const
{
  return HeroInfo->GetHeroData(CurrentHeroName);
}

FString UAuraGameInstance::GetSlotName(int32 SlotIndex) const
{
  return FString::Printf(TEXT("%s%d"), *SLOT_DEFAULT_NAME, SlotIndex);
}
