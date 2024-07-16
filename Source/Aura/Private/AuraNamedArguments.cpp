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

	NamedArguments.Effect_0 = FString("$Effect_0");
	NamedArguments.Effect_1 = FString("$Effect_1");
	NamedArguments.EffectPercent_0 = FString("$EffectPercent_0");
	NamedArguments.EffectPercent_1 = FString("$EffectPercent_1");
	NamedArguments.Duration_0 = FString("$Duration_0");
	NamedArguments.Duration_1 = FString("$Duration_1");
	NamedArguments.Period = FString("$Period");

	NamedArguments.AdditionalHitCount_0 = FString("$AdditionalHitCount_0");
	NamedArguments.AdditionalHitCount_1 = FString("$AdditionalHitCount_1");
	NamedArguments.HitEffectChange_0 = FString("$HitEffectChange_0");
	NamedArguments.HitEffectChange_1 = FString("$HitEffectChange_1");
	NamedArguments.ActorDuration_0 = FString("$ActorDuration_0");
	NamedArguments.ActorDuration_1 = FString("$ActorDuration_1");
	NamedArguments.ActorPeriod_0 = FString("$ActorPeriod_0");
	NamedArguments.ActorPeriod_1 = FString("$ActorPeriod_1");
}
