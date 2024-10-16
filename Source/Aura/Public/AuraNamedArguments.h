// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FStatusEffectsArgs
{
  FString Effect_0;
  FString Effect_1;
  FString EffectPercent_0;
  FString EffectPercent_1;
  FString EffectPercentFromResult_0;
  FString EffectPercentFromResult_1;
  FString Duration_0;
  FString Duration_1;
  FString Stacks_0;
  FString Stacks_1;
};

struct FUpgradeArgs
{
  FString Percent_0;
  FString Percent_1;
  FString Value_0;
  FString Value_1;
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
	
	FString Level_0;
	FString Level_1;
	
	FString Mana_0;
	FString Mana_1;
	FString CD_0;
	FString CD_1;
	
	FString Dmg_0;
	FString Dmg_1;

  FString Period;
	FString AdditionalHitCount_0;
	FString AdditionalHitCount_1;
	FString HitEffectChange_0;
	FString HitEffectChange_1;
	FString ActorDuration_0;
	FString ActorDuration_1;
	FString ActorPeriod_0;
	FString ActorPeriod_1;

  /* UPGRADE */

  FString Percent_0;
  FString Percent_1;

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

  TArray<FStatusEffectsArgs> StatusEffectsArgs;
  TArray<FUpgradeArgs> UpgradeArgs;

private:
	static FAuraNamedArguments NamedArguments;
};
