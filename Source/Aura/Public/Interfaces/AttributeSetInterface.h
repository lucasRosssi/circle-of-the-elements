// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeSetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttributeSetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IAttributeSetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetPower();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetHealth();
	
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  float GetAttackSpeed();
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  float GetCastSpeed();
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  float GetMovementSpeed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementSpeed(float InAttackSpeed);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTimeDilation(float InTimeDilation);
};
