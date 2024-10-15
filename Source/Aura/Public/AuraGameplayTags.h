// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	bool IsValid() const { return bIsValid; }
  static const FAuraGameplayTags& Get();
  static void InitializeNativeGameplayTags();

	FGameplayTag DifficultyClass;
	FGameplayTag DifficultyClass_Initial;
	FGameplayTag DifficultyClass_Easy;
	FGameplayTag DifficultyClass_Normal;
	FGameplayTag DifficultyClass_Hard;
	FGameplayTag DifficultyClass_Extreme;
	FGameplayTag DifficultyClass_Special;
	
	FGameplayTag Attributes;
	FGameplayTag Attributes_Primary;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Constitution;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Wisdom;
	FGameplayTag Attributes_Primary_Charisma;

	FGameplayTag Attributes_Vital;
	FGameplayTag Attributes_Vital_MaxHealth;
	FGameplayTag Attributes_Vital_MaxMana;
	
	FGameplayTag Attributes_Secondary;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ActionSpeed;
	FGameplayTag Attributes_Secondary_CooldownReduction;
	FGameplayTag Attributes_Secondary_CriticalRate;
	FGameplayTag Attributes_Secondary_CriticalDamage;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
  FGameplayTag Attributes_Secondary_ParryChance;
  FGameplayTag Attributes_Secondary_Restoration;

	FGameplayTag Attributes_Resistance;
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Energy;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Necrotic;

	FGameplayTag Attributes_Special;
	FGameplayTag Attributes_Special_Power;

	FGameplayTag Attributes_Meta;
	FGameplayTag Attributes_Meta_IncomingXP;
	FGameplayTag Attributes_Meta_IncomingForce;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Energy;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Necrotic;

  FGameplayTag Heal;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

  /* STATUS EFFECTS */

	FGameplayTag StatusEffects;
  FGameplayTag StatusEffects_Duration;
  FGameplayTag StatusEffects_Stacks;
	
	FGameplayTag StatusEffects_Invulnerable;
	FGameplayTag StatusEffects_ManaRegenOff;

  // Buffs

	FGameplayTag StatusEffects_Buff;
	FGameplayTag StatusEffects_Buff_Shield;
	FGameplayTag StatusEffects_Buff_Haste;

  // Debuffs
	
	FGameplayTag StatusEffects_Debuff;
	FGameplayTag StatusEffects_Debuff_Bleeding;
	FGameplayTag StatusEffects_Debuff_Burning;
	FGameplayTag StatusEffects_Debuff_Chill;
	FGameplayTag StatusEffects_Debuff_Charm;
	FGameplayTag StatusEffects_Debuff_Weakness;
	FGameplayTag StatusEffects_Debuff_Exposed;
	FGameplayTag StatusEffects_Debuff_Root;
	FGameplayTag StatusEffects_Debuff_Shock;

  // Incapacitations

	FGameplayTag StatusEffects_Incapacitation;
	FGameplayTag StatusEffects_Incapacitation_HitReact;
	FGameplayTag StatusEffects_Incapacitation_Knockback;
	FGameplayTag StatusEffects_Incapacitation_Stun;
	FGameplayTag StatusEffects_Incapacitation_Freeze;
	FGameplayTag StatusEffects_Incapacitation_Sleep;
	FGameplayTag StatusEffects_Incapacitation_Paralysis;

  // Conditions

	FGameplayTag StatusEffects_Condition;
	FGameplayTag StatusEffects_Condition_Alive;
	FGameplayTag StatusEffects_Condition_Dead;
	FGameplayTag StatusEffects_Condition_Reanimated;
	FGameplayTag StatusEffects_Condition_Possessed;

  // Immunities

	FGameplayTag Immunities;

	FGameplayTag Immunities_Debuff;
	FGameplayTag Immunities_Debuff_Burning;
	FGameplayTag Immunities_Debuff_Chill;
	FGameplayTag Immunities_Debuff_Charm;
	
	FGameplayTag Immunities_Incapacitation;
	FGameplayTag Immunities_Incapacitation_Stun;
	FGameplayTag Immunities_Incapacitation_HitReact;
	FGameplayTag Immunities_Incapacitation_Knockback;
	FGameplayTag Immunities_Incapacitation_Freeze;

  /* BOSS */

	FGameplayTag Boss;
	FGameplayTag Boss_State;
	FGameplayTag Boss_State_1st;
	FGameplayTag Boss_State_2nd;
	FGameplayTag Boss_State_3rd;
	FGameplayTag Boss_State_4th;
	FGameplayTag Boss_State_Ability;

  /* INPUTS */

	FGameplayTag InputTag;
	FGameplayTag InputTag_Primary;
	FGameplayTag InputTag_Secondary;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Dodge;

	FGameplayTag InputTag_Confirm;
	FGameplayTag InputTag_Cancel;
	
	FGameplayTag InputTag_AttributesMenu;
	FGameplayTag InputTag_SkillsMenu;

  /* ABILITIES */

	FGameplayTag Abilities;
	FGameplayTag Abilities_NONE;
	FGameplayTag Cooldown;
	FGameplayTag Charges;

	FGameplayTag Abilities_Type;
	FGameplayTag Abilities_Type_Active;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Triggered;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Mobility;
  
  FGameplayTag Abilities_HealthRegeneration;

	FGameplayTag Abilities_Tier;
	FGameplayTag Abilities_Tier_I;
	FGameplayTag Abilities_Tier_II;
	FGameplayTag Abilities_Tier_III;
	FGameplayTag Abilities_Tier_IV;
	FGameplayTag Abilities_Tier_V;

	FGameplayTag Abilities_Status;
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	FGameplayTag Abilities_Status_Native;

	FGameplayTag Abilities_Element;
	FGameplayTag Abilities_Element_None;
	FGameplayTag Abilities_Element_Physical;
	FGameplayTag Abilities_Element_Arcane;
	FGameplayTag Abilities_Element_Fire;
	FGameplayTag Abilities_Element_Ice;
	FGameplayTag Abilities_Element_Lightning;
	FGameplayTag Abilities_Element_Necrotic;
	FGameplayTag Abilities_Element_Duo;
  
	// Reactions
	
	FGameplayTag Abilities_Reaction;
  
	FGameplayTag Abilities_Reaction_HitReact;
	FGameplayTag Abilities_Reaction_Stun;
	FGameplayTag Abilities_Reaction_Charm;
	FGameplayTag Abilities_Reaction_Shock;
  FGameplayTag Abilities_Reaction_Paralysis;
  
	FGameplayTag Abilities_Reaction_ShieldStackRemove;
  FGameplayTag Abilities_Reaction_SecondWind;

	// Actions
	
	FGameplayTag Abilities_Action;
	FGameplayTag Abilities_Action_Dodge;
	FGameplayTag Cooldown_Action_Dodge;
	FGameplayTag Charges_Action_Dodge;

  // Interactions

  FGameplayTag Abilities_Interaction;
  FGameplayTag Abilities_Interaction_SoulEssence;
  FGameplayTag Abilities_Interaction_ElementalEssence;

	// Common Active
	
	FGameplayTag Abilities_Active;
	FGameplayTag Abilities_Active_Primary;
	FGameplayTag Abilities_Active_Secondary;
	FGameplayTag Abilities_Active_3rd;
	FGameplayTag Abilities_Active_4th;
	FGameplayTag Abilities_Active_5th;
	FGameplayTag Abilities_Active_6th;

	/* Aura */
	
	FGameplayTag Abilities_Active_Aura;

	// Fire
	
	FGameplayTag Abilities_Active_Aura_Fireball;
	FGameplayTag Cooldown_Active_Aura_Fireball;
	FGameplayTag Charges_Active_Aura_Fireball;
	FGameplayTag Abilities_Active_Aura_FireMissiles;
	FGameplayTag Cooldown_Active_Aura_FireMissiles;

	// Lightning
	
	FGameplayTag Abilities_Active_Aura_ChainLightning;
	FGameplayTag Cooldown_Active_Aura_ChainLightning;
	FGameplayTag Abilities_Active_Aura_MagneticShot;
	FGameplayTag Cooldown_Active_Aura_MagneticShot;
	FGameplayTag Charges_Active_Aura_MagneticShot;
	FGameplayTag Abilities_Active_Aura_ThunderBall;
	FGameplayTag Cooldown_Active_Aura_ThunderBall;
	FGameplayTag Abilities_Active_Aura_LightningMissiles;
	FGameplayTag Cooldown_Active_Aura_LightningMissiles;

	// Ice
	
	FGameplayTag Abilities_Active_Aura_FrostRay;
	FGameplayTag Cooldown_Active_Aura_FrostRay;
	FGameplayTag Abilities_Active_Aura_Blizzard;
	FGameplayTag Cooldown_Active_Aura_Blizzard;
	FGameplayTag Abilities_Active_Aura_FrostBolt;
	FGameplayTag Cooldown_Active_Aura_FrostBolt;
	FGameplayTag Abilities_Active_Aura_IceMissiles;
	FGameplayTag Cooldown_Active_Aura_IceMissiles;
	FGameplayTag Charges_Active_Aura_IceMissiles;

	// Arcane
	
	FGameplayTag Abilities_Active_Aura_ArcaneMissiles;
	FGameplayTag Cooldown_Active_Aura_ArcaneMissiles;
	FGameplayTag Abilities_Active_Aura_ArcaneShards;
	FGameplayTag Cooldown_Active_Aura_ArcaneShards;
	FGameplayTag Abilities_Active_Aura_CharmingHex;
	FGameplayTag Cooldown_Active_Aura_CharmingHex;
	FGameplayTag Abilities_Active_Aura_ArcaneBolt;
	FGameplayTag Cooldown_Active_Aura_ArcaneBolt;

	// Necrotic

	FGameplayTag Abilities_Active_Aura_NecroBolt;
	FGameplayTag Cooldown_Active_Aura_NecroBolt;
	FGameplayTag Abilities_Active_Aura_NecroMissiles;
	FGameplayTag Cooldown_Active_Aura_NecroMissiles;
	
	// Physical

	FGameplayTag Abilities_Active_Aura_WindGust;
	FGameplayTag Cooldown_Active_Aura_WindGust;
	FGameplayTag Abilities_Active_Aura_RockMissiles;
	FGameplayTag Cooldown_Active_Aura_RockMissiles;

	/* Vilkar */

	FGameplayTag Abilities_Active_Vilkar;

	FGameplayTag Abilities_Active_Summon;

  FGameplayTag Abilities_Passive;
  FGameplayTag Abilities_Passive_Common;
  FGameplayTag Abilities_Passive_Upgrade;
  FGameplayTag Abilities_Passive_Upgrade_SoulAbsorption;
  FGameplayTag Abilities_Passive_Upgrade_Respite;
  FGameplayTag Abilities_Passive_Upgrade_SecondWind;
	
	FGameplayTag Abilities_Passive_Aura;
	FGameplayTag Abilities_Passive_Aura_EnergyShield;
	FGameplayTag Cooldown_Passive_Aura_EnergyShield;
	FGameplayTag Abilities_Passive_Aura_LifeSiphon;
	FGameplayTag Abilities_Passive_Aura_SpiritSiphon;

	FGameplayTag Abilities_Passive_Vilkar;

  FGameplayTag Abilities_Event;
  FGameplayTag Abilities_Event_SecondWindTrigger;

  /* UPGRADES */

  FGameplayTag Upgrades;
  
  FGameplayTag Upgrades_Soul;
  FGameplayTag Upgrades_Soul_Absorption;
  FGameplayTag Upgrades_Soul_Respite;
  FGameplayTag Upgrades_Soul_Brilliance;
  FGameplayTag Upgrades_Soul_SecondWind;
  
  FGameplayTag Upgrades_Fire;
  FGameplayTag Upgrades_Fire_Absorption;
  
  FGameplayTag Upgrades_Ice;
  FGameplayTag Upgrades_Ice_Absorption;
  
  FGameplayTag Upgrades_Lightning;
  FGameplayTag Upgrades_Lightning_Absorption;
  
  FGameplayTag Upgrades_Necrotic;
  FGameplayTag Upgrades_Necrotic_Absorption;
  
  FGameplayTag Upgrades_Arcane;
  FGameplayTag Upgrades_Arcane_Absorption;
  
  FGameplayTag Upgrades_Physical;
  FGameplayTag Upgrades_Physical_Absorption;

  /* RESOURCES */
	
	FGameplayTag Resources;

	FGameplayTag Resources_Point;
	FGameplayTag Resources_Point_Perk;
	
	FGameplayTag Resources_Arcanite;
	
	FGameplayTag Resources_Essence;
	FGameplayTag Resources_Essence_Soul;
	FGameplayTag Resources_Essence_Physical;
	FGameplayTag Resources_Essence_Arcane;
	FGameplayTag Resources_Essence_Fire;
	FGameplayTag Resources_Essence_Ice;
	FGameplayTag Resources_Essence_Lightning;
	FGameplayTag Resources_Essence_Necrotic;

	TMap<FGameplayTag, FGameplayTag> EssenceToAbility;

	TMap<FGameplayTag, TArray<FGameplayTag>> ParentsToChildren;

private:
	static FAuraGameplayTags GameplayTags;

	bool bIsValid = false;
};
