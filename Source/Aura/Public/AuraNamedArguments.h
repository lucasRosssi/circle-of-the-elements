// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FAbilityGenericArgs
{
  FString Percent;
  FString Percent_;
  FString Value;
  FString Value_;
};

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
	FString Level_;
	
	FString Mana;
	FString Mana_;
	FString CD;
	FString CD_;
	
	FString Dmg;
	FString Dmg_;

	/* HELPERS - Common used texts */

	FString FireDmg;
	FString IceDmg;
	FString LightningDmg;
	FString ArcaneDmg;
	FString NecroticDmg;
	FString PhysicalDmg;
	FString NextFireDmg;
	FString NextIceDmg;
	FString NextLightningDmg;
	FString NextArcaneDmg;
	FString NextNecroticDmg;
	FString NextPhysicalDmg;

	TMap<FGameplayTag, TTuple<FString, FText>> DamageTypeTexts;
	TMap<FGameplayTag, TTuple<FString, FText>> NextDamageTypeTexts;

  TArray<FAbilityGenericArgs> AbilityGenericArgs;

private:
	static FAuraNamedArguments NamedArguments;
};
