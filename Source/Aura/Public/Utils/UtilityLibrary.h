// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Vectors")
	static float GetDistance(const FVector& V1, const FVector& V2);

	UFUNCTION(BlueprintPure, Category="Vectors")
	static FVector GetMiddlePoint(const FVector& V1, const FVector& V2);
};
