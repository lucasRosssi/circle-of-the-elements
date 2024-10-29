// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "MainMenuWidgetController.generated.h"

struct FSaveInfo;
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
  void NewGameStart(const FSaveInfo& SaveData);
  UFUNCTION(BlueprintCallable)
  void DeleteSlotConfirmed(int32 SlotIndex);
  UFUNCTION(BlueprintCallable)
  void LoadSlotAndPlay(int32 SlotIndex);

  UFUNCTION(BlueprintPure)
  UAuraSaveGame* GetSaveSlotData(int32 SlotIndex);

  UFUNCTION(BlueprintPure)
  void GetSelectedSaveData(FString& OutPlayerName, int32& OutSlotIndex);
  UFUNCTION(BlueprintCallable)
  void SetSelectedSaveData(const FString& InPlayerName, int32 InSlotIndex);
  
protected:

private:
  UPROPERTY()
  TArray<UAuraSaveGame*> SaveSlots;

  FString SelectedPlayerName = FString();
  int32 SelectedSlotIndex = -1;
};
