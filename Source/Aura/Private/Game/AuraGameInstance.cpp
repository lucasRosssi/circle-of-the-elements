// Copyright Lucas Rossi


#include "Game/AuraGameInstance.h"

#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Character/Data/HeroInfo.h"
#include "Game/AuraSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"

void UAuraGameInstance::SaveGameData(const FSaveInfo& SaveData)
{
  const FString SlotName = GetSlotName(SaveData.SlotIndex);
  if (UGameplayStatics::DoesSaveGameExist(SlotName, SaveData.SlotIndex))
  {
    UGameplayStatics::DeleteGameInSlot(SlotName, SaveData.SlotIndex);
  }
  
  USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UAuraSaveGame::StaticClass());
  UAuraSaveGame* AuraSaveGame = Cast<UAuraSaveGame>(SaveGameObject);
  AuraSaveGame->SaveInfo = SaveData;
  AuraSaveGame->SaveInfo.SlotName = SlotName;

  UGameplayStatics::SaveGameToSlot(AuraSaveGame, SlotName, SaveData.SlotIndex);

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

void UAuraGameInstance::LoadAndPlay(int32 SlotIndex)
{
  const UAuraSaveGame* AuraSave = LoadGameData(SlotIndex);

  if (!AuraSave)
  {
    const FString SlotName = GetSlotName(SlotIndex);
    UE_LOG(
      LogAura,
      Error,
      TEXT("Trying to load a nonexistent save file: %s."),
      *SlotName
    )
    return;
  }

  const TSoftObjectPtr<UWorld> Level = RegionInfo->GetLevels()[AuraSave->SaveInfo.RegionName];

  UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
}

FHeroData UAuraGameInstance::GetCurrentHeroData() const
{
  return HeroInfo->GetHeroData(CurrentHeroName);
}

FString UAuraGameInstance::GetSlotName(int32 SlotIndex) const
{
  return FString::Printf(TEXT("%s%d"), *SLOT_DEFAULT_NAME, SlotIndex);
}
