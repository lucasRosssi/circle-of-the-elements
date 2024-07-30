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
  static const FAuraGameplayTags& Get() { return GameplayTags; }
  static void InitializeNativeGameplayTags();

	FGameplayTag DifficultyClass;
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

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	FGameplayTag StatusEffects;
	FGameplayTag StatusEffects_Duration;
	
	FGameplayTag StatusEffects_Invulnerable;
	FGameplayTag StatusEffects_ManaRegenOff;

	FGameplayTag StatusEffects_Buff;
	FGameplayTag StatusEffects_Buff_Shield;
	
	FGameplayTag StatusEffects_Debuff;
	FGameplayTag StatusEffects_Debuff_Bleeding;
	FGameplayTag StatusEffects_Debuff_Burning;
	FGameplayTag StatusEffects_Debuff_Chill;
	FGameplayTag StatusEffects_Debuff_Charm;

	FGameplayTag StatusEffects_Incapacitation;
	FGameplayTag StatusEffects_Incapacitation_HitReact;
	FGameplayTag StatusEffects_Incapacitation_Knockback;
	FGameplayTag StatusEffects_Incapacitation_Stun;
	FGameplayTag StatusEffects_Incapacitation_Freeze;

	FGameplayTag StatusEffects_Condition;
	FGameplayTag StatusEffects_Condition_Alive;
	FGameplayTag StatusEffects_Condition_Dead;
	FGameplayTag StatusEffects_Condition_Reanimated;
	FGameplayTag StatusEffects_Condition_Possessed;

	FGameplayTag Immunities;
	
	FGameplayTag Immunities_Incapacitation;
	FGameplayTag Immunities_Incapacitation_Stun;
	FGameplayTag Immunities_Incapacitation_HitReact;
	FGameplayTag Immunities_Incapacitation_Knockback;

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

	FGameplayTag Abilities;
	FGameplayTag Abilities_NONE;
	FGameplayTag Cooldown;
	FGameplayTag Charges;

	FGameplayTag Abilities_Type;
	FGameplayTag Abilities_Type_Active;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Triggered;
	FGameplayTag Abilities_Type_None;

	FGameplayTag Abilities_Status;
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	FGameplayTag Abilities_Status_Native;
	
	FGameplayTag Abilities_Reaction;
	FGameplayTag Abilities_Reaction_HitReact;
	FGameplayTag Abilities_Reaction_Stun;
	FGameplayTag Abilities_Reaction_ShieldStackRemove;
	FGameplayTag Abilities_Reaction_Charm;
	
	FGameplayTag Abilities_Action;
	FGameplayTag Abilities_Action_Dodge;
	FGameplayTag Cooldown_Action_Dodge;
	FGameplayTag Charges_Action_Dodge;

	FGameplayTag Abilities_Active;
	FGameplayTag Abilities_Active_Primary;
	FGameplayTag Abilities_Active_Secondary;

	FGameplayTag Abilities_Active_Aura;
	
	FGameplayTag Abilities_Active_Aura_Fireball;
	FGameplayTag Cooldown_Active_Aura_Fireball;
	FGameplayTag Charges_Active_Aura_Fireball;
	
	FGameplayTag Abilities_Active_Aura_ChainLightning;
	FGameplayTag Cooldown_Active_Aura_ChainLightning;

	FGameplayTag Abilities_Active_Aura_FrostRay;
	FGameplayTag Cooldown_Active_Aura_FrostRay;
	FGameplayTag Abilities_Active_Aura_Blizzard;
	FGameplayTag Cooldown_Active_Aura_Blizzard;
	
	FGameplayTag Abilities_Active_Aura_ArcaneMissiles;
	FGameplayTag Cooldown_Active_Aura_ArcaneMissiles;
	FGameplayTag Abilities_Active_Aura_ArcaneShards;
	FGameplayTag Cooldown_Active_Aura_ArcaneShards;
	FGameplayTag Abilities_Active_Aura_CharmingHex;
	FGameplayTag Cooldown_Active_Aura_CharmingHex;

	FGameplayTag Abilities_Active_Vilkar;

	FGameplayTag Abilities_Active_Summon;

	FGameplayTag Abilities_Passive;
	
	FGameplayTag Abilities_Passive_Aura;
	FGameplayTag Abilities_Passive_Aura_EnergyShield;
	FGameplayTag Cooldown_Passive_Aura_EnergyShield;
	FGameplayTag Abilities_Passive_Aura_LifeSiphon;
	FGameplayTag Abilities_Passive_Aura_SpiritSiphon;

	FGameplayTag Abilities_Passive_Vilkar;

	TMap<FGameplayTag, TArray<FGameplayTag>> ParentsToChildren;

private:
	static FAuraGameplayTags GameplayTags;
};
