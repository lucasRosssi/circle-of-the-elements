// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipperInterface.generated.h"

class URune;
class USpirit;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipperInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IEquipperInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  bool Equip(const FGuid& ID, int32 Slot);
};
