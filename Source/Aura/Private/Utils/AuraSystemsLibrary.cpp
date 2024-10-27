// Copyright Lucas Rossi


#include "Utils/AuraSystemsLibrary.h"

#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/CameraManager.h"
#include "Managers/CombatManager.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

UCharacterInfo* UAuraSystemsLibrary::GetCharacterClassInfo(
  const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetCharacterInfo();
}

UHeroInfo* UAuraSystemsLibrary::GetHeroInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetHeroInfo();
}

UAbilityInfo* UAuraSystemsLibrary::GetAbilitiesInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetAbilityInfo();
}

UStatusEffectInfo* UAuraSystemsLibrary::GetStatusEffectInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetStatusEffectInfo();
}

UUpgradeInfo* UAuraSystemsLibrary::GetUpgradeInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetUpgradeInfo();
}

URegionInfo* UAuraSystemsLibrary::GetRegionInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetRegionInfo();
}

URewardsInfo* UAuraSystemsLibrary::GetRewardsInfo(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetRewardsInfo();
}

UAbilityManager* UAuraSystemsLibrary::GetAbilityManager(const UObject* WorldContextObject)
{
	const AGameModeBase* GameMode = GetManagerInterfaceGameMode(WorldContextObject);

  if (!GameMode) return nullptr;

  UAbilityManager* AbilityManager = IManagerInterface::Execute_GetAbilityManager(GameMode);

  if (!AbilityManager)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't have an Ability Manager. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
  }
  
	return AbilityManager;
}

ACameraManager* UAuraSystemsLibrary::GetCameraManager(const UObject* WorldContextObject)
{
  ACameraManager* CameraManager = CastChecked<ACameraManager>(
    UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0)
  );

  return CameraManager;
}

UUpgradeManager* UAuraSystemsLibrary::GetUpgradeManager(const UObject* WorldContextObject)
{
  const AGameModeBase* GameMode = GetManagerInterfaceGameMode(WorldContextObject);

  if (!GameMode) return nullptr;

  UUpgradeManager* UpgradeManager = IManagerInterface::Execute_GetUpgradeManager(GameMode);

  if (!UpgradeManager)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't have an Upgrade Manager. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
  }
  
  return UpgradeManager;
}

ULocationManager* UAuraSystemsLibrary::GetLocationManager(const UObject* WorldContextObject)
{
  const AGameModeBase* GameMode = GetManagerInterfaceGameMode(WorldContextObject);

  if (!GameMode) return nullptr;

  ULocationManager* LocationManager = IManagerInterface::Execute_GetLocationManager(GameMode);

  if (!LocationManager)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't have an Location Manager. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
  }
  
  return LocationManager;
}

UCombatManager* UAuraSystemsLibrary::GetCombatManager(const UObject* WorldContextObject)
{
  const AGameModeBase* GameMode = GetManagerInterfaceGameMode(WorldContextObject);

  if (!GameMode) return nullptr;

  UCombatManager* CombatManager = IManagerInterface::Execute_GetCombatManager(GameMode);

  if (!CombatManager)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't have an Combat Manager. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
  }
  
  return CombatManager;
}

URewardManager* UAuraSystemsLibrary::GetRewardManager(const UObject* WorldContextObject)
{
  const AGameModeBase* GameMode = GetManagerInterfaceGameMode(WorldContextObject);

  if (!GameMode) return nullptr;

  URewardManager* RewardManager = IManagerInterface::Execute_GetRewardManager(GameMode);

  if (!RewardManager)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't have an Reward Manager. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
  }
  
  return RewardManager;
}

UUIManager* UAuraSystemsLibrary::GetUIManager(const UObject* WorldContextObject, int32 PlayerIndex)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex);
	if (const AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(PlayerController))
	{
		return AuraPlayerController->GetUIManager();
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Player index is invalid or doesn't have an UI Manager. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}

AAuraHUD* UAuraSystemsLibrary::GetAuraHUD(const UObject* WorldContextObject, int32 PlayerIndex)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
	{
		return AuraGameMode->GetAuraHUD(PlayerIndex);
	}

	UE_LOG(LogAura, Warning, TEXT(
		"Player index is invalid or doesn't have an Aura HUD. "
		"Trying to access from object: %s"),
		*WorldContextObject->GetName()
		);

	return nullptr;
}

FHeroData UAuraSystemsLibrary::GetCurrentHeroData(const UObject* WorldContextObject)
{
  AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);

  if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(GameMode))
  {
    return AuraGameMode->GetCurrentHeroData();
  }

  return FHeroData();
}

void UAuraSystemsLibrary::SaveGameData(
  const UObject* WorldContextObject,
  const FSaveInfo& SaveData
  )
{
  UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  AuraGameInstance->SaveGameData(SaveData);
}

void UAuraSystemsLibrary::SaveGameByObject(const UObject* WorldContextObject, UAuraSaveGame* SaveGameObject)
{
  UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  AuraGameInstance->SaveGameByObject(SaveGameObject);
}

UAuraSaveGame* UAuraSystemsLibrary::LoadGameData(
  const UObject* WorldContextObject,
  int32 SlotIndex
)
{
  UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->LoadGameData(SlotIndex);
}

void UAuraSystemsLibrary::DeleteGameData(const UObject* WorldContextObject, int32 SlotIndex)
{
  UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->DeleteGameData(SlotIndex);
}

void UAuraSystemsLibrary::LoadGameAndPlay(const UObject* WorldContextObject, int32 SlotIndex)
{
  UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->LoadAndPlay(SlotIndex);
}

UAuraSaveGame* UAuraSystemsLibrary::GetCurrentSaveGameObject(const UObject* WorldContextObject)
{
  const UAuraGameInstance* AuraGameInstance = CastChecked<UAuraGameInstance>(
    UGameplayStatics::GetGameInstance(WorldContextObject)
  );

  return AuraGameInstance->GetCurrentSaveGameObject();
}

AGameModeBase* UAuraSystemsLibrary::GetManagerInterfaceGameMode(const UObject* WorldContextObject)
{
  AGameModeBase* GameMode = UGameplayStatics::GetGameMode(WorldContextObject);

  if (!GameMode)
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current world doesn't have a GameMode. "
      "Trying to access from object: %s"),
      *WorldContextObject->GetName()
      );
    return nullptr;
  }
  
  if (!GameMode->Implements<UManagerInterface>())
  {
    UE_LOG(LogAura, Warning, TEXT(
      "[AuraSystemsLibrary] Current Game Mode doesn't implement Manager Interface. "
      "Trying to access from object: %s. "
      "Game Mode: %s"),
      *WorldContextObject->GetName(),
      *GameMode->GetName()
      );
    return nullptr;
  }

  return GameMode;
}
