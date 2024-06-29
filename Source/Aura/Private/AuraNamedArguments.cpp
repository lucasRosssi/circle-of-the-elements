// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

FAuraNamedArguments FAuraNamedArguments::NamedArguments;

void FAuraNamedArguments::InitializeNamedArguments()
{
	NamedArguments.Level_0 = FString("$Level_0");
	NamedArguments.Level_1 = FString("$Level_1");
	
	NamedArguments.Mana_0 = FString("$Mana_0");
	NamedArguments.Mana_1 = FString("$Mana_1");
	NamedArguments.CD_0 = FString("$CD_0");
	NamedArguments.CD_1 = FString("$CD_1");
	
	NamedArguments.Dmg_0 = FString("$Dmg_0");
	NamedArguments.Dmg_1 = FString("$Dmg_1");

	NamedArguments.Duration_0 = FString("$Duration_0");
	NamedArguments.Duration_1 = FString("$Duration_1");
	NamedArguments.Period = FString("$Period");

	NamedArguments.BounceCount_0 = FString("$BounceCount_0");
	NamedArguments.BounceCount_1 = FString("$BounceCount_1");
}
