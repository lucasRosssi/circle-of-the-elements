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

void ULoadoutWidgetController::OnEquipped(UEquipment* Equipment)
{
  if (!Equipment) return;
  
  OnEquippedDelegate.Broadcast(Equipment);
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

  Spirit->Equip(Slot);
}

void ULoadoutWidgetController::RemoveEquipment(UEquipment* InEquipment)
{
  if (!InEquipment || !GetAuraPlayerState()) return;

  InEquipment->Unequip();
  AuraPlayerState->RemoveEquipmentFromInventory(InEquipment);
}

TArray<USpirit*> ULoadoutWidgetController::GetPlayerSpirits()
{
  if (!GetAuraPlayerState()) return TArray<USpirit*>();
  
  return AuraPlayerState->GetSpiritsInventory();
}

TArray<URune*> ULoadoutWidgetController::GetPlayerRunes()
{
  if (!GetAuraPlayerState()) return TArray<URune*>();
  
  return AuraPlayerState->GetRunesInventory();
}

TMap<int32, FGuid> ULoadoutWidgetController::GetPlayerLoadout()
{
  if (!GetAuraPlayerState()) return TMap<int32, FGuid>();
  
  return AuraPlayerState->GetLoadout();
}
