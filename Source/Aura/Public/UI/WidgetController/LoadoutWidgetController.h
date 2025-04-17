// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "LoadoutWidgetController.generated.h"

class URune;
class USpirit;
class UEquipment;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
  FEquipmentGlobeSelected,
  UEquipment*,
  SelectedEquipment
);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
  FOnEquipped,
  UEquipment*,
  Equipment
);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API ULoadoutWidgetController : public UAuraWidgetController
{
  GENERATED_BODY()

public:
  virtual void BroadcastInitialValues() override;
  virtual void BindCallbacksToDependencies() override;

  UPROPERTY(BlueprintAssignable)
  FEquipmentGlobeSelected EquipmentGlobeSelectedDelegate;
  UPROPERTY(BlueprintAssignable)
  FOnEquipped OnEquippedDelegate;

  UFUNCTION(BlueprintCallable)
  void EquipmentGlobeSelected(UEquipment* Equipment);
  UFUNCTION(BlueprintCallable)
  void OnEquipped(UEquipment* Equipment);

  UFUNCTION(BlueprintCallable)
  FString GetEquipmentDescription(UEquipment* Equipment);

  UFUNCTION(BlueprintCallable)
  void EquipToInputPressed(USpirit* Spirit, const int32 Slot);

  UFUNCTION(BlueprintPure)
  TArray<USpirit*> GetPlayerSpirits();
  UFUNCTION(BlueprintPure)
  TArray<URune*> GetPlayerRunes();
  UFUNCTION(BlueprintPure)
  TMap<int32, FGuid> GetPlayerLoadout();

private:
};
