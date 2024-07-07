// Copyright Lucas Rossi


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * PRIMARY ATTRIBUTES
	 */

	GameplayTags.Attributes = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes"),
		FString("Attribute tags")
		);

	GameplayTags.Attributes_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary"),
		FString("Primary attribute tags")
		);
	
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

	GameplayTags.Attributes_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary"),
		FString("Secondary attribute tags")
		);

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

	GameplayTags.Attributes_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Special"),
			FString("Special attribute tags")
			);
	
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

	GameplayTags.Attributes_Meta = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta"),
		FString("Meta attribute tags")
		);

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	/*
	 * INPUT TAGS
	 */

	GameplayTags.InputTag = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag"),
		FString("Input tags")
		);

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
		FString("Damage tags")
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

	GameplayTags.Attributes_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance"),
		FString("Resistance attribute tags")
		);

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
	 * Status Effects Tags
	 */
	
	GameplayTags.StatusEffects = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects"),
		FString("Status Effect tags")
		);

	GameplayTags.StatusEffects_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Duration"),
		FString("Duration for the status effect")
		);


	GameplayTags.StatusEffects_Invulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Invulnerable"),
		FString("Invulnerability effect")
		);

	GameplayTags.StatusEffects_ManaRegenOff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.ManaRegenOff"),
		FString("Effect that stops mana regen in some situations (ongoing spending, few seconds after spending)")
		);

	// Buffs

	GameplayTags.StatusEffects_Buff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Buff"),
		FString("Buff effect tags")
		);

	GameplayTags.StatusEffects_Buff_Shield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Buff.Shield"),
		FString("Blocks limited instances of damage")
		);

	// Debuffs

	GameplayTags.StatusEffects_Debuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Debuff"),
		FString("Debuff effect tags")
		);

	GameplayTags.StatusEffects_Debuff_Bleeding = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Debuff.Bleeding"),
		FString("Bleeding effect. Slow overtime damage. Stacks when applied and when moving.")
		);

	GameplayTags.StatusEffects_Debuff_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Debuff.Burning"),
		FString("Burning effect. Quick overtime damage. Can stack.")
		);

	GameplayTags.StatusEffects_Debuff_Chill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Debuff.Chill"),
		FString("Chill effect. Reduces movement and actions speeds. Can stack.")
		);

	// Incapacitations

	GameplayTags.StatusEffects_Incapacitation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Incapacitation"),
		FString("Incapacitation effects tags. Interrupt and block most actions.")
		);

	GameplayTags.StatusEffects_Incapacitation_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Incapacitation.HitReact"),
		FString("Hit reaction effect.")
		);
	
	GameplayTags.StatusEffects_Incapacitation_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Incapacitation.Knockback"),
		FString("Knockback effect. Gets pushed away.")
		);
	
	GameplayTags.StatusEffects_Incapacitation_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Incapacitation.Stun"),
		FString("Stun effect. Can't do anything. Temporarily resist new applications after recovery.")
		);

	
	 // Conditions
	 
	GameplayTags.StatusEffects_Condition = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Condition"),
		FString("Condition tags")
		);

	GameplayTags.StatusEffects_Condition_Alive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Condition.Alive"),
		FString("Alive condition. Entity can interact with the world normally.")
		);

	GameplayTags.StatusEffects_Condition_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Condition.Dead"),
		FString("Dead condition. Entity cannot interact with world, but can be revived or reanimated.")
		);

	GameplayTags.StatusEffects_Condition_Reanimated = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Condition.Reanimated"),
		FString("Reanimated tag. Entity was dead, but brought back to 'life' under someone else's control.")
		);

	GameplayTags.StatusEffects_Condition_Possessed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Condition.Possessed"),
		FString("Possessed tag. Entity is under someone else's control.")
		);

	/*
	 * Immunities Tags
	 */

	GameplayTags.Immunities = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities"),
		FString("Immunity tags")
		);

	GameplayTags.Immunities_Incapacitation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Incapacitation"),
		FString("Immunity to incapacitation effects")
		);

	GameplayTags.Immunities_Incapacitation_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Incapacitation.Stun"),
		FString("Immunity to stun effect")
		);

	GameplayTags.Immunities_Incapacitation_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Incapacitation.HitReact"),
		FString("Immunity to hit react effect")
		);

	GameplayTags.Immunities_Incapacitation_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Incapacitation.Knockback"),
		FString("Immunity to knockback effect")
		);
	
	/*
	 * Abilities Tags
	 */

	GameplayTags.Abilities = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities"),
		FString("Ability tags")
		);

	GameplayTags.Abilities_NONE = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.NONE"),
		FString("No ability.")
		);

	GameplayTags.Cooldown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown"),
		FString("Cooldown tags")
		);

	GameplayTags.Charges = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Charges"),
		FString("Charges tags")
		);

	// Type

	GameplayTags.Abilities_Type = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type"),
		FString("Type of ability tags")
		);

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

	GameplayTags.Abilities_Status= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status"),
		FString("Status of ability tags")
		);

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
	
	// Reactions

	GameplayTags.Abilities_Reaction = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Reaction"),
			FString("Reaction ability tags")
			);
	
	GameplayTags.Abilities_Reaction_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reaction.HitReact"),
		FString("Hit reaction ability")
		);

	GameplayTags.Abilities_Reaction_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reaction.Stun"),
		FString("Get stunned ability")
		);

	GameplayTags.Abilities_Reaction_ShieldStackRemove = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reaction.ShieldStackRemove"),
		FString("Removes stacks of shield if applicable")
		);

	// Actions

	GameplayTags.Abilities_Action = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Action"),
		FString("Action ability tags")
		);
	
	GameplayTags.Abilities_Action_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Action.Dodge"),
		FString("Dodge ability")
		);

	GameplayTags.Cooldown_Action_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Action.Dodge"),
		FString("Dodge ability cooldown")
		);

	GameplayTags.Charges_Action_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Charges.Action.Dodge"),
		FString("Dodge ability charges")
		);

	// Attacks
	
	GameplayTags.Abilities_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active"),
		FString("Attack ability tags")
		);
	
	GameplayTags.Abilities_Active_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Primary"),
		FString("Primary attack ability")
		);

	GameplayTags.Abilities_Active_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Secondary"),
		FString("Secondary attack ability")
		);

	// Aura active abilities
	
	GameplayTags.Abilities_Active_Aura = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura"),
		FString("Aura active abilities")
		);

	GameplayTags.Abilities_Active_Aura_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.Fireball"),
		FString("Fireball ability")
		);

	GameplayTags.Cooldown_Active_Aura_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.Fireball"),
		FString("Fireball ability cooldown")
		);

	GameplayTags.Charges_Active_Aura_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Charges.Active.Aura.Fireball"),
		FString("Fireball ability charges")
		);

	GameplayTags.Abilities_Active_Aura_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.ChainLightning"),
		FString("ChainLightning ability")
		);

	GameplayTags.Cooldown_Active_Aura_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ChainLightning"),
		FString("ChainLightning ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_ArcaneMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.ArcaneMissiles"),
		FString("ArcaneMissiles ability")
		);

	GameplayTags.Cooldown_Active_Aura_ArcaneMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ArcaneMissiles"),
		FString("ArcaneMissiles ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.ArcaneShards"),
		FString("ArcaneShards ability")
		);

	GameplayTags.Cooldown_Active_Aura_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ArcaneShards"),
		FString("ArcaneShards ability cooldown")
		);

	// Vilkar active abilities

	GameplayTags.Abilities_Active_Vilkar = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Vilkar"),
		FString("Vilkar active abilities")
		);

	// Summoning

	GameplayTags.Abilities_Active_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Summon"),
		FString("Summon ability")
		);

	// Passive

	GameplayTags.Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive"),
		FString("Passive abilities")
		);

	// Aura passive abilities

	GameplayTags.Abilities_Passive_Aura = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Aura"),
		FString("Aura passive abilities")
		);

	GameplayTags.Abilities_Passive_Aura_EnergyShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Aura.EnergyShield"),
		FString("Energy Shield ability")
		);

	GameplayTags.Cooldown_Passive_Aura_EnergyShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Passive.Aura.EnergyShield"),
		FString("Energy Shield cooldown")
		);


	GameplayTags.Abilities_Passive_Aura_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Aura.LifeSiphon"),
		FString("Life Siphon ability")
		);

	
	GameplayTags.Abilities_Passive_Aura_SpiritSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Aura.SpiritSiphon"),
		FString("Spirit Siphon ability")
		);

	// Vilkar passive abilities

	GameplayTags.Abilities_Passive_Vilkar = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Vilkar"),
		FString("Vilkar passive abilities")
		);

	/*
	 * Map of Tags to their children
	 */

	const TArray BuffTags({
		GameplayTags.StatusEffects_Buff_Shield
	});
	const TArray DebuffTags({
		GameplayTags.StatusEffects_Debuff_Bleeding,
		GameplayTags.StatusEffects_Debuff_Burning,
		GameplayTags.StatusEffects_Debuff_Chill,
	});
	const TArray IncapacitationTags({
		GameplayTags.StatusEffects_Incapacitation_HitReact,
		GameplayTags.StatusEffects_Incapacitation_Knockback,
		GameplayTags.StatusEffects_Incapacitation_Stun,
	});
	const TArray ConditionTags({
		GameplayTags.StatusEffects_Condition_Alive,
		GameplayTags.StatusEffects_Condition_Dead,
		GameplayTags.StatusEffects_Condition_Reanimated,
		GameplayTags.StatusEffects_Condition_Possessed,
	});
	TArray<FGameplayTag> StatusEffectTags;
	StatusEffectTags.Append(BuffTags);
	StatusEffectTags.Append(DebuffTags);
	StatusEffectTags.Append(IncapacitationTags);
	StatusEffectTags.Append(ConditionTags);
	
	GameplayTags.ParentsToChildren.Add(GameplayTags.StatusEffects, StatusEffectTags);
	GameplayTags.ParentsToChildren.Add(GameplayTags.StatusEffects_Buff, BuffTags);
	GameplayTags.ParentsToChildren.Add(GameplayTags.StatusEffects_Debuff, DebuffTags);
	GameplayTags.ParentsToChildren.Add(GameplayTags.StatusEffects_Incapacitation, IncapacitationTags);
	GameplayTags.ParentsToChildren.Add(GameplayTags.StatusEffects_Condition, ConditionTags);
}
