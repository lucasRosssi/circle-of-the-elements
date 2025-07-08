// Copyright Lucas Rossi


#include "AuraNamedArguments.h"

#include "AuraGameplayTags.h"

FAuraNamedArguments FAuraNamedArguments::NamedArguments;

void FAuraNamedArguments::InitializeNamedArguments()
{
	NamedArguments.Level = FString("$Level");
	NamedArguments.Level_ = FString("$Level_");
	
	NamedArguments.Mana = FString("$Mana");
	NamedArguments.Mana_ = FString("$Mana_");
	NamedArguments.CD = FString("$CD");
	NamedArguments.CD_ = FString("$CD_");

	
	NamedArguments.Dmg = FString("$Dmg");
	NamedArguments.Dmg_ = FString("$Dmg_");

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
		{NamedArguments.FireDmg, FText::FromString("<Fire>{$Dmg} fire</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Water,
		{NamedArguments.IceDmg, FText::FromString("<Ice>{$Dmg} ice</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Lightning,
		{NamedArguments.LightningDmg, FText::FromString("<Lightning>{$Dmg} lightning</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Chaos,
		{NamedArguments.ArcaneDmg, FText::FromString("<Arcane>{$Dmg} arcane</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Earth,
		{NamedArguments.NecroticDmg, FText::FromString("<Necrotic>{$Dmg} necrotic</> damage")}
		);
	NamedArguments.DamageTypeTexts.Add(
		AuraTags.Damage_Air,
		{NamedArguments.PhysicalDmg, FText::FromString("<Physical>{$Dmg} physical</> damage")}
		);
	
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Fire,
		{NamedArguments.NextFireDmg, FText::FromString("<Old>{$Dmg}</> > <Fire>{$Dmg_} fire</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Water,
		{NamedArguments.NextIceDmg, FText::FromString("<Old>{$Dmg}</> > <Ice>{$Dmg_} ice</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Lightning,
		{NamedArguments.NextLightningDmg, FText::FromString("<Old>{$Dmg}</> > <Lightning>{$Dmg_} lightning</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Chaos,
		{NamedArguments.NextArcaneDmg, FText::FromString("<Old>{$Dmg}</> > <Arcane>{$Dmg_} arcane</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Earth,
		{NamedArguments.NextNecroticDmg, FText::FromString("<Old>{$Dmg}</> > <Necrotic>{$Dmg_} necrotic</> damage")}
		);
	NamedArguments.NextDamageTypeTexts.Add(
		AuraTags.Damage_Air,
		{NamedArguments.NextPhysicalDmg, FText::FromString("<Old>{$Dmg}</> > <Physical>{$Dmg_} physical</> damage")}
		);

  FAbilityGenericArgs AbilityArgs1;
  AbilityArgs1.Percent = FString("$Percent");
  AbilityArgs1.Percent_ = FString("$Percent_");
  AbilityArgs1.Value = FString("$Value");
  AbilityArgs1.Value_ = FString("$Value_");

  FAbilityGenericArgs AbilityArgs2;
  AbilityArgs2.Percent = FString("$2Percent");
  AbilityArgs2.Percent_ = FString("$2Percent_");
  AbilityArgs2.Value = FString("$2Value");
  AbilityArgs2.Value_ = FString("$2Value_");

  FAbilityGenericArgs AbilityArgs3;
  AbilityArgs3.Percent = FString("$3Percent");
  AbilityArgs3.Percent_ = FString("$3Percent_");
  AbilityArgs3.Value = FString("$3Value");
  AbilityArgs3.Value_ = FString("$3Value_");

  FAbilityGenericArgs AbilityArgs4;
  AbilityArgs4.Percent = FString("$4Percent");
  AbilityArgs4.Percent_ = FString("$4Percent_");
  AbilityArgs4.Value = FString("$4Value");
  AbilityArgs4.Value_ = FString("$4Value_");

  FAbilityGenericArgs AbilityArgs5;
  AbilityArgs5.Percent = FString("$5Percent");
  AbilityArgs5.Percent_ = FString("$5Percent_");
  AbilityArgs5.Value = FString("$5Value");
  AbilityArgs5.Value_ = FString("$5Value_");

  NamedArguments.AbilityGenericArgs.Add(AbilityArgs1);
  NamedArguments.AbilityGenericArgs.Add(AbilityArgs2);
  NamedArguments.AbilityGenericArgs.Add(AbilityArgs3);
  NamedArguments.AbilityGenericArgs.Add(AbilityArgs4);
  NamedArguments.AbilityGenericArgs.Add(AbilityArgs5);
}
