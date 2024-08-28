// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraSystemsLibrary.generated.h"

class URewardManagerComponent;
class UEncounterManagerComponent;
/**
 * 
 */
UCLASS()
class AURA_API UAuraSystemsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Aura Systems|Encounter")
	static UEncounterManagerComponent* GetEncounterManager(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="Aura Systems|Encounter")
	static URewardManagerComponent* GetRewardManager(const UObject* WorldContextObject);
};
