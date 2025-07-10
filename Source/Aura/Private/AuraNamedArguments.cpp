// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

FAuraNamedArguments FAuraNamedArguments::NamedArguments;

void FAuraNamedArguments::InitializeNamedArguments()
{
	NamedArguments.Level = FString("Level");
	
	NamedArguments.Mana = FString("Mana");
	NamedArguments.CD = FString("CD");
  
	NamedArguments.Dmg = FString("Dmg");

  for (int32 i = 1; i <= 10; i++)
  {
    NamedArguments.AbilityGenericArgs.Add(FString::Printf(TEXT("v%d"), i));
    NamedArguments.AbilityPluralArgs.Add(FString::Printf(TEXT("v%d_s"), i));
  }
}
