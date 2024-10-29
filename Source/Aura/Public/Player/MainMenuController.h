// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UINavController.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMainMenuController : public AUINavController
{
	GENERATED_BODY()

public:
  AMainMenuController();

  UPROPERTY()
  FString PlayerName;
  UPROPERTY()
  FString SaveSlotName;
  UPROPERTY()
  int32 SlotIndex;

protected:
  virtual void BeginPlay() override;

private:
};
