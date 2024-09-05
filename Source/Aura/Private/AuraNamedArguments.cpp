// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

#include "AuraGameplayTags.h"

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
	NamedArguments.EffectPercentFromResult_0 = FString("$EffectPercentFromResult_0");
	NamedArguments.EffectPercentFromResult_1 = FString("$EffectPercentFromResult_1");
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

	NamedArguments.FireDmg = FString("$FireDmg");
	NamedArguments.IceDmg = FString("$IceDmg");
	NamedArguments.LightningDmg = FString("$LightningDmg");
	NamedArguments.ArcaneDmg = FString("$ArcaneDmg");
	NamedArguments.NecroticDmg = FString("$NecroticDmg");
	NamedArguments.PhysicalDmg = FString("$PhysicalDmg");
	NamedArguments.NextFireDmg = FString("$NextFireDmg");
	NamedArguments.NextIceDmg = FString("$NextIceDmg");
	NamedArguments.NextLightningDmg = FString("$NextLightningDmg");
	NamedArguments.NextArcaneDmg = FString("$NextArcaneDmg");
	NamedArguments.NextNecroticDmg = FString("$NextNecroticDmg");
	NamedArguments.NextPhysicalDmg = FString("$NextPhysicalDmg");

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Fire,
		{NamedArguments.FireDmg, TEXT("<Fire>{$Dmg_0} fire</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Ice,
		{NamedArguments.IceDmg, TEXT("<Ice>{$Dmg_0} ice</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Lightning,
		{NamedArguments.LightningDmg, TEXT("<Lightning>{$Dmg_0} lightning</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Energy,
		{NamedArguments.ArcaneDmg, TEXT("<Arcane>{$Dmg_0} arcane</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Necrotic,
		{NamedArguments.NecroticDmg, TEXT("<Necrotic>{$Dmg_0} necrotic</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Physical,
		{NamedArguments.PhysicalDmg, TEXT("<Physical>{$Dmg_0} physical</> damage")}
		);
	
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Fire,
		{NamedArguments.NextFireDmg, TEXT("<Old>{$Dmg_0}</> > <Fire>{$Dmg_1} fire</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Ice,
		{NamedArguments.NextIceDmg, TEXT("<Old>{$Dmg_0}</> > <Ice>{$Dmg_1} ice</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Lightning,
		{NamedArguments.NextLightningDmg, TEXT("<Old>{$Dmg_0}</> > <Lightning>{$Dmg_1} lightning</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Energy,
		{NamedArguments.NextArcaneDmg, TEXT("<Old>{$Dmg_0}</> > <Arcane>{$Dmg_1} arcane</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Necrotic,
		{NamedArguments.NextNecroticDmg, TEXT("<Old>{$Dmg_0}</> > <Necrotic>{$Dmg_1} necrotic</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Physical,
		{NamedArguments.NextPhysicalDmg, TEXT("<Old>{$Dmg_0}</> > <Physical>{$Dmg_1} physical</> damage")}
		);
}
