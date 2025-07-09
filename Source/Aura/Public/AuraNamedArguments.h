// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

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

	/* VALUES */
	
	FString Level;
	
	FString Mana;
	FString CD;
	
	FString Dmg;

  TArray<FString> AbilityGenericArgs;

private:
	static FAuraNamedArguments NamedArguments;
};
