// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "MainMenuWidgetController.generated.h"

class UAuraSaveGame;
class UMainMenuWidget;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UMainMenuWidgetController : public UObject
{
	GENERATED_BODY()

public:
  void InitializeSaveSlots();

  UFUNCTION(BlueprintCallable)
  void NewSlotPressed(const FString& EnteredName, int32 SlotIndex);
  UFUNCTION(BlueprintCallable)
  void DeleteSlotConfirmed(int32 SlotIndex);

  UFUNCTION(BlueprintPure)
  UAuraSaveGame* GetSaveSlotData(int32 SlotIndex);
  
protected:

private:
  UPROPERTY()
  TArray<UAuraSaveGame*> SaveSlots;
};
