// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"

/**
 * AuraNamedArguments
 *
 * Template for the named arguments used in the project
 */

struct FAuraNamedArguments
{
public:
	static const FAuraNamedArguments& Get() { return NamedArguments; }
	static void InitializeNamedArguments();

	FString Level_0;
	FString Level_1;
	
	FString Mana_0;
	FString Mana_1;
	FString CD_0;
	FString CD_1;
	
	FString Dmg_0;
	FString Dmg_1;

	FString Effect_0;
	FString Effect_1;
	FString EffectPercent_0;
	FString EffectPercent_1;
	FString Duration_0;
	FString Duration_1;
	FString Period;

	FString AdditionalHitCount_0;
	FString AdditionalHitCount_1;
	FString HitEffectChange_0;
	FString HitEffectChange_1;
	FString ActorDuration_0;
	FString ActorDuration_1;
	FString ActorPeriod_0;
	FString ActorPeriod_1;

private:
	static FAuraNamedArguments NamedArguments;
};
