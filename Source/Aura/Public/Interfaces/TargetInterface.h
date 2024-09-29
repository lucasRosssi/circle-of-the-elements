// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	static bool Implements(const UObject*);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HighlightActor(AActor* InstigatorActor = nullptr);
	static void SafeExec_HighlightActor(UObject* , AActor* InstigatorActor = nullptr);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnHighlightActor();
	static void SafeExec_UnHighlightActor(UObject*);
};
