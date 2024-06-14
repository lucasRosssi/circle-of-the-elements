// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

FAuraNamedArguments FAuraNamedArguments::NamedArguments;

void FAuraNamedArguments::InitializeNamedArguments()
{
	NamedArguments.Level_0 = FString("$Level0");
	NamedArguments.Level_1 = FString("$Level1");
	
	NamedArguments.Mana_0 = FString("$Mana0");
	NamedArguments.Mana_1 = FString("$Mana1");
	NamedArguments.CD_0 = FString("$CD0");
	NamedArguments.CD_1 = FString("$CD1");
	
	NamedArguments.FDmg_0 = FString("$FDmg0");
	NamedArguments.FDmg_1 = FString("$FDmg1");
}
