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

	GameplayTags.Attributes_Special_CooldownReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Special.CooldownReduction"),
		FString("Percentage of cooldown time reduced")
		);

	/*
	 * META ATTRIBUTES
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	/*
	 * INPUT TAGS
	 */

	GameplayTags.InputTag_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Primary"),
		FString("Input Tag for Primary action")
		);

	GameplayTags.InputTag_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Secondary"),
		FString("Input Tag for Secondary action")
		);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for Skill 1")
		);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for Skill 2")
		);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for Skill 3")
		);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for Skill 4")
		);

	GameplayTags.InputTag_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Dodge"),
		FString("Input Tag for dodging")
		);

	GameplayTags.InputTag_Confirm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Confirm"),
		FString("Input Tag for confirming actions or UI selections")
		);

	GameplayTags.InputTag_Cancel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Cancel"),
		FString("Input Tag for canceling actions or going back from UI menus")
		);

	GameplayTags.InputTag_AttributesMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.AttributesMenu"),
		FString("Input Tag for toggling the attributes menu")
		);

	GameplayTags.InputTag_SkillsMenu = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SkillsMenu"),
		FString("Input Tag for toggling the skills menu")
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

	GameplayTags.Damage_Necrotic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Necrotic"),
		FString("Necrotic damage type")
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

	GameplayTags.Attributes_Resistance_Necrotic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Necrotic"),
		FString("Resistance to Necrotic damage")
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

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Necrotic, 
		GameplayTags.Attributes_Resistance_Necrotic
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

	GameplayTags.Abilities_NONE = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NONE"),
		FString("No ability.")
		);

	// Type

	GameplayTags.Abilities_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Active"),
		FString("Active type ability. Must receive an input to activate.")
		);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Passive type ability. Active at all times if equipped.")
		);

	GameplayTags.Abilities_Type_Triggered = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Triggered"),
		FString("Triggered type ability. Activates when receiving a non input trigger (result of an effect).")
		);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("No type ability.")
		);

	// Status

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Ability is locked and can't be learned.")
		);

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Ability is locked, but can be learned.")
		);

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Ability is unlocked, but is not equipped and can't be used.")
		);

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Ability is equipped and can be used.")
		);

	GameplayTags.Abilities_Status_Native = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Native"),
		FString("Ability is native and can always be used. This status is not supposed to change.")
		);
	
	// Attacks

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit reaction ability")
		);
	
	GameplayTags.Abilities_Action_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Action.Dodge"),
		FString("Dodge ability")
		);

	GameplayTags.Cooldown_Action_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Action.Dodge"),
		FString("Dodge ability cooldown")
		);
	
	GameplayTags.Abilities_Attack_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.Primary"),
		FString("Primary attack ability")
		);

	GameplayTags.Abilities_Attack_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.Secondary"),
		FString("Secondary attack ability")
		);

	GameplayTags.Abilities_Attack_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.Fireball"),
		FString("Fireball ability")
		);

	GameplayTags.Cooldown_Attack_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Attack.Fireball"),
		FString("Fireball ability cooldown")
		);

	GameplayTags.Abilities_Attack_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack.ChainLightning"),
		FString("ChainLightning ability")
		);

	GameplayTags.Cooldown_Attack_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Attack.ChainLightning"),
		FString("ChainLightning ability cooldown")
		);

	// Summoning

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon ability")
		);
}
