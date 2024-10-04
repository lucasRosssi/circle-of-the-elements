// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "InteractInterface.generated.h"

class UInteractComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	static bool Implements(const UObject* Object);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(const AController* Controller);
	static void Safe_Interact(UObject* Object, const AController* Controller);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  UInteractComponent* GetInteractComponent() const;
  static UInteractComponent* Safe_GetInteractComponent(const UObject* Object);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  FGameplayEventData GetAbilityEventData() const;
  static FGameplayEventData Safe_GetAbilityEventData(const UObject* Object);
};
