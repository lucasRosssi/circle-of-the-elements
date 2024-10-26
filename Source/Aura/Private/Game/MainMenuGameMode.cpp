// Copyright Lucas Rossi


#include "Game/MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
  Super::BeginPlay();

  if (MainMenuMusic)
  {
    UGameplayStatics::PlaySound2D(
      this,
      MainMenuMusic
      );
  }
}
