// Copyright Lucas Rossi


#include "UI/WidgetController/LoadoutWidgetController.h"

#include "AbilitySystem/Equipment/Spirit.h"
#include "Player/AuraPlayerState.h"

void ULoadoutWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void ULoadoutWidgetController::BindCallbacksToDependencies()
{
	
}

void ULoadoutWidgetController::EquipmentGlobeSelected(UEquipment* Equipment)
{
  if (!Equipment) return;
  
  EquipmentGlobeSelectedDelegate.Broadcast(Equipment);
}

FString ULoadoutWidgetController::GetEquipmentDescription(UEquipment* Equipment)
{
  return Equipment->GetEquipmentDescription();
}

void ULoadoutWidgetController::EquipToInputPressed(
	USpirit* Spirit,
	const int32 Slot
	)
{
	if (!Spirit) return;

  Spirit->Equip(GetAuraPlayerState(), Slot);
}

TArray<USpirit*> ULoadoutWidgetController::GetPlayerSpirits()
{
  return TArray<USpirit*>();
}

TArray<URune*> ULoadoutWidgetController::GetPlayerRunes()
{
  return TArray<URune*>();
}

TMap<FGameplayTag, FGuid> ULoadoutWidgetController::GetPlayerLoadout()
{
  return TMap<FGameplayTag, FGuid>();
}
