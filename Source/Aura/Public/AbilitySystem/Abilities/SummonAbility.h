// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "SummonAbility.generated.h"

class AAuraCharacterBase;
/**
 * 
 */
UCLASS()
class AURA_API USummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<AAuraCharacterBase> GetRandomMinionClass();

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	AAuraCharacterBase* SpawnMinion(FVector Location);

	/* Number of minions to spawn per summon ability activation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Summoning")
	int32 MinionsPerSummon = 5;

	/* Maximum number of minions that can be active at the same time */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Summoning")
	int32 MaxMinions = 5;

	/* Count of currently active minions */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Summoning")
	int32 ActiveMinions = 0;

	/* Types of minions that can be summoned */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<AAuraCharacterBase>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;

protected:
	UFUNCTION(BlueprintPure, Category = "Summoning")
	bool HasMaxMinionsActive();
};
