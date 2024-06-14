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
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Toughness;
	FGameplayTag Attributes_Primary_Agility;
	FGameplayTag Attributes_Primary_Finesse;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;
	
  FGameplayTag Attributes_Secondary_Armor;
  FGameplayTag Attributes_Secondary_ActionSpeed;
  FGameplayTag Attributes_Secondary_MovementSpeed;
  FGameplayTag Attributes_Secondary_CriticalRate;
  FGameplayTag Attributes_Secondary_CriticalDamage;
  FGameplayTag Attributes_Secondary_HealthRegeneration;
  FGameplayTag Attributes_Secondary_ManaRegeneration;
  FGameplayTag Attributes_Secondary_StaminaRegeneration;
  FGameplayTag Attributes_Secondary_StaminaEfficiency;

	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Energy;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Necrotic;

	FGameplayTag Attributes_Special_ParryChance;
	FGameplayTag Attributes_Special_CooldownReduction;

	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Energy;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Necrotic;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	
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
	
	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_Invulnerable;

	FGameplayTag Abilities_NONE;

	FGameplayTag Abilities_Type_Active;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Triggered;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	FGameplayTag Abilities_Status_Native;


	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_Action_Dodge;
	FGameplayTag Cooldown_Action_Dodge;
	
	FGameplayTag Abilities_Attack_Primary;
	FGameplayTag Abilities_Attack_Secondary;
	FGameplayTag Abilities_Attack_Fireball;
	FGameplayTag Cooldown_Attack_Fireball;
	FGameplayTag Abilities_Attack_ChainLightning;
	FGameplayTag Cooldown_Attack_ChainLightning;

	FGameplayTag Abilities_Summon;

private:
	static FAuraGameplayTags GameplayTags;
};
