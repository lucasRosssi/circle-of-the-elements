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
	
	FString FDmg_0;
	FString FDmg_1;

private:
	static FAuraNamedArguments NamedArguments;
};
