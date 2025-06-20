// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IWidgetControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void AssignWidgetController(UObject* InWidgetController);
};
