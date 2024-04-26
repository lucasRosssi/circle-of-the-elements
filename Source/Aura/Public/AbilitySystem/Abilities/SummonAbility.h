// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API USummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	/* Number of minions to spawn per summon ability activation */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 MinionsPerSummon = 5;

	/* Maximum number of minions that can be active at the same time */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 MaxMinions = 5;

	/* Types of minions that can be summoned */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
};
