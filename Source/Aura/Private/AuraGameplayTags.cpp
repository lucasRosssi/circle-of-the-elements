// Copyright Lucas Rossi


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

const FAuraGameplayTags& FAuraGameplayTags::Get()
{
	if (GameplayTags.IsValid()) return GameplayTags;

	InitializeNativeGameplayTags();
	return GameplayTags;
}

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	if (GameplayTags.IsValid()) return;
	/*
	 * DIFFICULTY CLASSES
	 */

	GameplayTags.DifficultyClass = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass"),
			FString("Difficulty Class tags")
			);

	GameplayTags.DifficultyClass_Easy = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass.Easy"),
			FString("Difficulty Class tags")
			);

	GameplayTags.DifficultyClass_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass.Normal"),
			FString("Difficulty Class tags")
			);
	GameplayTags.DifficultyClass_Hard = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass.Hard"),
			FString("Difficulty Class tags")
			);
	GameplayTags.DifficultyClass_Extreme = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass.Extreme"),
			FString("Difficulty Class tags")
			);
	GameplayTags.DifficultyClass_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("DifficultyClass.Special"),
			FString("Difficulty Class tags")
			);
	
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
		FString("Increases Armor and Incapacitation Resistance")
		);
	
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Primary.Dexterity"),
			FString("Increases Action Speed and Parry Chance")
			);
	
	GameplayTags.Attributes_Primary_Constitution = UGameplayTagsManager::Get().AddNativeGameplayTag(
      		FName("Attributes.Primary.Constitution"),
      		FString("Increases Maximum Health and Health Recovery")
      		);
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases Maximum Mana and Mana Regeneration")
		);

	GameplayTags.Attributes_Primary_Wisdom = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Wisdom"),
		FString("Increases Critical Rate and Critical Damage")
		);

	GameplayTags.Attributes_Primary_Charisma = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Charisma"),
		FString("Increases Cooldown Reduction")
		);

	/*
	 * VITAL ATTRIBUTES
	 */

	GameplayTags.Attributes_Vital = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Vital"),
			FString("Vital attributes")
			);

	GameplayTags.Attributes_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Vital.MaxHealth"),
			FString("Maximum amount of health")
			);

	GameplayTags.Attributes_Vital_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.MaxMana"),
		FString("Maximum amount of mana")
		);
	
	/*
	 * SECONDARY ATTRIBUTES
	 */

	GameplayTags.Attributes_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary"),
		FString("Secondary attribute tags")
		);
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken")
		);

	GameplayTags.Attributes_Secondary_ActionSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ActionSpeed"),
		FString("Movement, attack, cast and skill speed")
		);
	
	GameplayTags.Attributes_Secondary_CooldownReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CooldownReduction"),
		FString("Percentage of cooldown time reduced")
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
	
	GameplayTags.Attributes_Secondary_ParryChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ParryChance"),
	FString("Chance to parry an attack and negate damage")
	);
	
	/*
	 * SPECIAL ATTRIBUTES
	 */

	GameplayTags.Attributes_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attributes.Special"),
			FString("Special attribute tags")
			);

	GameplayTags.Attributes_Special_Power = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Special.Power"),
		FString("Multiplies base damage dealt")
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
	GameplayTags.Attributes_Meta_IncomingForce = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingForce"),
		FString("Incoming Force Meta Attribute")
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

	GameplayTags.StatusEffects_Debuff_Charm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Debuff.Charm"),
		FString("Charm effect. Temporarily fights for the source team.")
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

	GameplayTags.StatusEffects_Incapacitation_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("StatusEffects.Incapacitation.Freeze"),
		FString("Freeze effect. Inflicted when having many stacks of Chill applied. Can't do anything. Temporarily resist new applications after recovery.")
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

	GameplayTags.Immunities_Debuff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Debuff"),
		FString("Immunity to debuff effects")
		);

	GameplayTags.Immunities_Debuff_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Debuff.Burning"),
		FString("Immunity to burning effect")
		);

	GameplayTags.Immunities_Debuff_Chill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Debuff.Chill"),
		FString("Immunity to chill effect")
		);

	GameplayTags.Immunities_Debuff_Charm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Debuff.Charm"),
		FString("Immunity to charm effect")
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

	GameplayTags.Immunities_Incapacitation_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Immunities.Incapacitation.Freeze"),
		FString("Immunity to freeze effect")
		);

	/*
	 * Boss Tags
	 */

	GameplayTags.Boss = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss"),
		FString("Boss tags")
		);

	GameplayTags.Boss_State = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State"),
		FString("Boss states tags")
		);

	GameplayTags.Boss_State_1st = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State.1st"),
		FString("Boss 1st state tag")
		);

	GameplayTags.Boss_State_2nd = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State.2nd"),
		FString("Boss 2nd state tag")
		);

	GameplayTags.Boss_State_3rd = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State.3rd"),
		FString("Boss 3rd state tag")
		);

	GameplayTags.Boss_State_4th = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State.4th"),
		FString("Boss 4th state tag")
		);

	GameplayTags.Boss_State_Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Boss.State.Ability"),
		FString("Boss state change ability tag")
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

	// Tiers

	GameplayTags.Abilities_Tier = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier"),
		FString("Ability tier tags.")
		);

	GameplayTags.Abilities_Tier_I = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier.I"),
		FString("Tier I abilities tag.")
		);

	GameplayTags.Abilities_Tier_II = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier.II"),
		FString("Tier II abilities tag.")
		);

	GameplayTags.Abilities_Tier_III = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier.III"),
		FString("Tier III abilities tag.")
		);

	GameplayTags.Abilities_Tier_IV = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier.IV"),
		FString("Tier IV abilities tag.")
		);

	GameplayTags.Abilities_Tier_V = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Tier.V"),
		FString("Tier V abilities tag.")
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

	// Elements

	GameplayTags.Abilities_Element = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element"),
		FString("Ability element tags.")
		);

	GameplayTags.Abilities_Element_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.None"),
		FString("Non-elemental ability tag.")
		);
	
	GameplayTags.Abilities_Element_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Physical"),
		FString("Physical element ability tag.")
		);

	GameplayTags.Abilities_Element_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Arcane"),
		FString("Arcane element ability tag.")
		);

	GameplayTags.Abilities_Element_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Fire"),
		FString("Fire element ability tag.")
		);
	
	GameplayTags.Abilities_Element_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Ice"),
		FString("Ice element ability tag.")
		);

	GameplayTags.Abilities_Element_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Lightning"),
		FString("Lightning element ability tag.")
		);

	GameplayTags.Abilities_Element_Necrotic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Necrotic"),
		FString("Necrotic element ability tag.")
		);

	GameplayTags.Abilities_Element_Duo = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Element.Duo"),
		FString("Two elements ability tag.")
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

	GameplayTags.Abilities_Reaction_Charm = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Reaction.Charm"),
		FString("Get charmed ability")
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

	GameplayTags.Abilities_Active_3rd = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.3rd"),
		FString("3rd attack ability")
		);

	GameplayTags.Abilities_Active_4th = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.4th"),
		FString("4th attack ability")
		);

	GameplayTags.Abilities_Active_5th = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.5th"),
		FString("5th attack ability")
		);

	GameplayTags.Abilities_Active_6th = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.6th"),
		FString("6th attack ability")
		);

	// Aura active abilities
	
	GameplayTags.Abilities_Active_Aura = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura"),
		FString("Aura active abilities")
		);

	// Aura Fire Abilities

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

	// Aura Lightning Abilities

	GameplayTags.Abilities_Active_Aura_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.ChainLightning"),
		FString("ChainLightning ability")
		);

	GameplayTags.Cooldown_Active_Aura_ChainLightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ChainLightning"),
		FString("ChainLightning ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_MagneticShot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.MagneticShot"),
		FString("MagneticShot ability")
		);

	GameplayTags.Cooldown_Active_Aura_MagneticShot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.MagneticShot"),
		FString("MagneticShot ability cooldown")
		);

	GameplayTags.Charges_Active_Aura_MagneticShot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Charges.Active.Aura.MagneticShot"),
		FString("MagneticShot ability charges")
		);

	GameplayTags.Abilities_Active_Aura_ThunderBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Active.Aura.ThunderBall"),
	FString("ThunderBall ability")
	);

	GameplayTags.Cooldown_Active_Aura_ThunderBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ThunderBall"),
		FString("ThunderBall ability cooldown")
		);

	// Aura Ice Abilities
	
	GameplayTags.Abilities_Active_Aura_FrostRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.FrostRay"),
		FString("FrostRay ability")
		);

	GameplayTags.Cooldown_Active_Aura_FrostRay = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.FrostRay"),
		FString("FrostRay ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_Blizzard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.Blizzard"),
		FString("Blizzard ability")
		);

	GameplayTags.Cooldown_Active_Aura_Blizzard = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.Blizzard"),
		FString("Blizzard ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_FrostBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Active.Aura.FrostBolt"),
	FString("FrostBolt ability")
	);

	GameplayTags.Cooldown_Active_Aura_FrostBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.FrostBolt"),
		FString("FrostBolt ability cooldown")
		);

	// Aura Arcane Abilities

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
	
	GameplayTags.Abilities_Active_Aura_CharmingHex = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Active.Aura.CharmingHex"),
		FString("CharmingHex ability")
		);

	GameplayTags.Cooldown_Active_Aura_CharmingHex = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.CharmingHex"),
		FString("CharmingHex ability cooldown")
		);

	GameplayTags.Abilities_Active_Aura_ArcaneBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Active.Aura.ArcaneBolt"),
	FString("ArcaneBolt ability")
	);

	GameplayTags.Cooldown_Active_Aura_ArcaneBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.ArcaneBolt"),
		FString("ArcaneBolt ability cooldown")
		);

	// Aura Necrotic Abilities

	GameplayTags.Abilities_Active_Aura_NecroBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Active.Aura.NecroBolt"),
	FString("NecroBolt ability")
	);

	GameplayTags.Cooldown_Active_Aura_NecroBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.NecroBolt"),
		FString("NecroBolt ability cooldown")
		);

	// Aura Physical Abilities

	GameplayTags.Abilities_Active_Aura_WindGust = UGameplayTagsManager::Get().AddNativeGameplayTag(
FName("Abilities.Active.Aura.WindGust"),
FString("WindGust ability")
);

	GameplayTags.Cooldown_Active_Aura_WindGust = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Active.Aura.WindGust"),
		FString("WindGust ability cooldown")
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
	 * Player Resources
	 */

	GameplayTags.Resources = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources"),
		FString("Resources")
		);

	GameplayTags.Resources_Point = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Point"),
		FString("Spendable points resources")
		);

	GameplayTags.Resources_Point_Perk = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Point.Perk"),
		FString("Perk point resource")
		);

	GameplayTags.Resources_Arcanite = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Arcanite"),
		FString("Arcanite crystal resource")
		);

	GameplayTags.Resources_Essence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence"),
		FString("Essences resources")
		);

	GameplayTags.Resources_Essence_Soul = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Soul"),
		FString("Soul essence resource")
		);

	GameplayTags.Resources_Essence_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Physical"),
		FString("Physical essence resource")
		);

	GameplayTags.Resources_Essence_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Arcane"),
		FString("Arcane essence resource")
		);

	GameplayTags.Resources_Essence_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Fire"),
		FString("Fire essence resource")
		);

	GameplayTags.Resources_Essence_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Ice"),
		FString("Ice essence resource")
		);

	GameplayTags.Resources_Essence_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
  	FName("Resources.Essence.Lightning"),
  	FString("Lightning essence resource")
  	);
  		
	GameplayTags.Resources_Essence_Necrotic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resources.Essence.Necrotic"),
		FString("Necrotic essence resource")
		);

	GameplayTags.EssenceToAbility.Add(
		GameplayTags.Resources_Essence_Physical,
		GameplayTags.Abilities_Element_Physical
		);
	GameplayTags.EssenceToAbility.Add(
		GameplayTags.Resources_Essence_Arcane,
		GameplayTags.Abilities_Element_Arcane
		);
	GameplayTags.EssenceToAbility.Add(
		GameplayTags.Resources_Essence_Fire,
		GameplayTags.Abilities_Element_Fire
		);
	GameplayTags.EssenceToAbility.Add(
		GameplayTags.Resources_Essence_Ice,
		GameplayTags.Abilities_Element_Ice
		);
	GameplayTags.EssenceToAbility.Add(
	GameplayTags.Resources_Essence_Lightning,
	GameplayTags.Abilities_Element_Lightning
	);
	GameplayTags.EssenceToAbility.Add(
		GameplayTags.Resources_Essence_Necrotic,
		GameplayTags.Abilities_Element_Necrotic
		);

	/*
	 * Map of Tags to their children
	 */

	const TArray SkillsInputTags({
		GameplayTags.InputTag_1,
		GameplayTags.InputTag_2,
		GameplayTags.InputTag_3,
		GameplayTags.InputTag_4
	});

	GameplayTags.ParentsToChildren.Add(GameplayTags.InputTag, SkillsInputTags);

	const TArray BuffTags({
		GameplayTags.StatusEffects_Buff_Shield
	});
	const TArray DebuffTags({
		GameplayTags.StatusEffects_Debuff_Bleeding,
		GameplayTags.StatusEffects_Debuff_Burning,
		GameplayTags.StatusEffects_Debuff_Chill,
		GameplayTags.StatusEffects_Debuff_Charm,
	});
	const TArray IncapacitationTags({
		GameplayTags.StatusEffects_Incapacitation_HitReact,
		GameplayTags.StatusEffects_Incapacitation_Knockback,
		GameplayTags.StatusEffects_Incapacitation_Stun,
		GameplayTags.StatusEffects_Incapacitation_Freeze
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

	const TArray ElementTags({
		GameplayTags.Abilities_Element_None,
		GameplayTags.Abilities_Element_Duo,
		GameplayTags.Abilities_Element_Physical,
		GameplayTags.Abilities_Element_Arcane,
		GameplayTags.Abilities_Element_Fire,
		GameplayTags.Abilities_Element_Ice,
		GameplayTags.Abilities_Element_Lightning,
		GameplayTags.Abilities_Element_Necrotic,
	});

	GameplayTags.ParentsToChildren.Add(GameplayTags.Abilities_Element, ElementTags);

	const TArray TierTags({
		GameplayTags.Abilities_Tier_I,
		GameplayTags.Abilities_Tier_II,
		GameplayTags.Abilities_Tier_III,
		GameplayTags.Abilities_Tier_IV,
		GameplayTags.Abilities_Tier_V,
	});

	GameplayTags.ParentsToChildren.Add(GameplayTags.Abilities_Tier, TierTags);

	GameplayTags.bIsValid = true;
}
