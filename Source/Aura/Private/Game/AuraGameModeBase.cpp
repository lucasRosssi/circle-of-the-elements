// Copyright Lucas Rossi


#include "Game/AuraGameModeBase.h"

#include "Character/AuraHero.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"

AAuraGameModeBase::AAuraGameModeBase()
{
  
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

FHeroData AAuraGameModeBase::GetCurrentHeroData()
{
  if (!CurrentHeroData.IsValid())
  {
    const UAuraGameInstance* AuraGI = GetAuraGameInstance();

    if (!AuraGI) return FHeroData();

    CurrentHeroData = AuraGI->GetCurrentHeroData();
  }

  return CurrentHeroData;
}

void AAuraGameModeBase::SpawnAndPossessSelectedHero()
{
  APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
  const AActor* PlayerStart = FindPlayerStart(PlayerController);

  FActorSpawnParameters SpawnParameters;
  SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
  AAuraHero* Hero = Cast<AAuraHero>(GetWorld()->SpawnActor(
    GetCurrentHeroData().HeroClass,
    &PlayerStart->GetActorTransform(),
    SpawnParameters
  ));

  if (!Hero)
  {
    UE_LOG(LogTemp, Error, TEXT("Unable to spawn Hero %hhd!"), GetCurrentHeroData().HeroName);
    return;
  }

  PlayerController->Possess(Hero);
  PlayerController->SetActorTickEnabled(true);

  Hero->SetActorLocation(PlayerStart->GetActorLocation(),
    false,
    nullptr,
    ETeleportType::ResetPhysics
    );
}
