// Copyright Lucas Rossi


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * PRIMARY ATTRIBUTES
	 */ 
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases melee damage and maximum health")
		);
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magic damage and maximum mana")
		);

	GameplayTags.Attributes_Primary_Toughness = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Toughness"),
		FString("Increases armor and maximum stamina")
		);

	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Primary.Agility"),
			FString("Increases movement speed and stamina efficiency")
			);

	GameplayTags.Attributes_Primary_Finesse = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Finesse"),
		FString("Increases action speed and critical rate")
		);

	/*
	 * SECONDARY ATTRIBUTES
	 */

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of health")
		);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of mana")
		);

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum amount of stamina")
		);
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken")
		);

	GameplayTags.Attributes_Secondary_ActionSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ActionSpeed"),
		FString("Attack, cast and skill speed")
		);

	GameplayTags.Attributes_Secondary_MovementSpeed = UGameplayTagsManager::Get()
	.AddNativeGameplayTag(
		FName("Attributes.Secondary.MovementSpeed"),
		FString("Running speed and dodge range")
		);

	GameplayTags.Attributes_Secondary_CriticalRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalRate"),
		FString("Chance to do a critical hit")
		);

	GameplayTags.Attributes_Secondary_CriticalDamage = UGameplayTagsManager::Get()
	.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalDamage"),
		FString("Percentage of damage a critical hit deals")
		);

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of health recovered over time")
		);
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get()
	.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of mana recovered over time")
		);

	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get()
	.AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegeneration"),
		FString("Amount of stamina recovered over time")
		);

	GameplayTags.Attributes_Secondary_StaminaEfficiency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaEfficiency"),
		FString("Stamina cost reduction")
		);

	/*
	 * SPECIAL ATTRIBUTES
	 */

	GameplayTags.Attributes_Special_ParryChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Special.ParryChance"),
		FString("Chance to parry an attack and negate damage")
		);

	/*
	 * INPUT TAGS
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
		);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
		);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
		);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
		);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
		);

	GameplayTags.InputTag_SpaceBar = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SpaceBar"),
		FString("Input Tag for Space Bar key")
		);

	/*
	 * Damage Tags
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical damage type")
		);
	
	GameplayTags.Damage_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Energy"),
		FString("Energy damage type")
		);

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire damage type")
		);

	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Ice"),
		FString("Ice damage type")
		);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning damage type")
		);
	
	/*
	 * Resistances Tags
	 */

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage")
		);

	GameplayTags.Attributes_Resistance_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Energy"),
		FString("Resistance to Energy damage")
		);

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage")
		);

	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Ice"),
		FString("Resistance to Ice damage")
		);

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
		);

	/*
	 * Map of Damage Types to Resistances
	 */

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Physical, 
		GameplayTags.Attributes_Resistance_Physical
	);
	
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Energy, 
		GameplayTags.Attributes_Resistance_Energy
	);

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Fire, 
		GameplayTags.Attributes_Resistance_Fire
	);

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Ice, 
		GameplayTags.Attributes_Resistance_Ice
	);

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Lightning, 
		GameplayTags.Attributes_Resistance_Lightning
	);
	
	/*
	 * Effects Tags
	 */

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Hit reaction effect")
		);

	GameplayTags.Effects_Invulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.Invulnerable"),
		FString("Invulnerability effect")
		);

	/*
	 * Abilities Tags
	 */

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit reaction ability")
		);
	
	GameplayTags.Abilities_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Dodge"),
		FString("Dodge ability")
		);
	
	GameplayTags.Abilities_Attack_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.Primary"),
		FString("Primary attack ability")
		);

	GameplayTags.Abilities_Attack_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.Secondary"),
		FString("Secondary attack ability")
		);
}
