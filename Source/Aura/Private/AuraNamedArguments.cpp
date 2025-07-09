// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

FAuraNamedArguments FAuraNamedArguments::NamedArguments;

void FAuraNamedArguments::InitializeNamedArguments()
{
	NamedArguments.Level = FString("Level");
	
	NamedArguments.Mana = FString("Mana");
	NamedArguments.CD = FString("CD");
  
	NamedArguments.Dmg = FString("Dmg");

  NamedArguments.AbilityGenericArgs.Add(FString("x"));
  NamedArguments.AbilityGenericArgs.Add(FString("x2"));
  NamedArguments.AbilityGenericArgs.Add(FString("x3"));
  NamedArguments.AbilityGenericArgs.Add(FString("x4"));
  NamedArguments.AbilityGenericArgs.Add(FString("x5"));
}
