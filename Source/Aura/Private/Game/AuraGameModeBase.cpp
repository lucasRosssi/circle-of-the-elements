// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "AuraGameplayTags.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/AbilityManager.h"
#include "Managers/EncounterManager.h"
#include "Managers/LocationManager.h"
#include "Managers/RewardManager.h"
#include "UI/HUD/AuraHUD.h"

AAuraGameModeBase::AAuraGameModeBase()
{
  AbilityManager = CreateDefaultSubobject<UAbilityManager>("Ability Manager");
  AbilityManager->SetGameMode(this);

  LocationManager = CreateDefaultSubobject<ULocationManager>("LocationManager");
  LocationManager->SetGameMode(this);

  EncounterManager = CreateDefaultSubobject<UEncounterManager>("EncounterManager");
  EncounterManager->SetGameMode(this);

  RewardManager = CreateDefaultSubobject<URewardManager>("RewardManager");
  RewardManager->SetGameMode(this);
}

void AAuraGameModeBase::OnNoAbilitiesLeft(const FGameplayTag& ElementTag)
{
  if (const FGameplayTag* EssenceTag = FAuraGameplayTags::Get().EssenceToAbility.FindKey(ElementTag))
  {
    RewardManager->RemoveRewardFromPool(*EssenceTag);
  }
  else
  {
    UE_LOG(
      LogAura,
      Warning,
      TEXT("Matching Essence Tag for %s not found! Failed to remove essence from the reward pool."),
      *ElementTag.ToString()
      )
  }
}

void AAuraGameModeBase::AddToXPStack(float InXP)
{
}

void AAuraGameModeBase::BeginPlay()
{
  Super::BeginPlay();

  GetOnEncounterFinishedDelegate().AddDynamic(RewardManager, &URewardManager::SpawnReward);
}

UAuraGameInstance* AAuraGameModeBase::GetAuraGameInstance()
{
  return Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
}

AAuraHUD* AAuraGameModeBase::GetAuraHUD(int32 PlayerIndex)
{
  if (!AuraHUDs.Contains(PlayerIndex))
  {
    AAuraHUD* AuraHUD = Cast<AAuraHUD>(
      UGameplayStatics::GetPlayerController(this, PlayerIndex)->GetHUD()
    );

    AuraHUDs.Add(PlayerIndex, AuraHUD);
  }

  return AuraHUDs[PlayerIndex];
}

void AAuraGameModeBase::LoadLevelInfo()
{
  LocationManager->InitLocation();
  EncounterManager->SetCurrentEncounterData();
  RewardManager->SetGatesRewards();
}

int32 AAuraGameModeBase::GetEnemiesLevel() const
{
  return EncounterManager->GetEnemiesLevel();
}

FOnExitLocation& AAuraGameModeBase::GetOnExitLocationDelegate()
{
  return LocationManager->OnExitLocationDelegate;
}

FOnEncounterFinished& AAuraGameModeBase::GetOnEncounterFinishedDelegate()
{
  return EncounterManager->OnEncounterFinishedDelegate;
}

FOnRewardTaken& AAuraGameModeBase::GetOnRewardTakenDelegate()
{
  return RewardManager->OnRewardTakenDelegate;
}
