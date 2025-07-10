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
  * HEROES
  */

  GameplayTags.Heroes = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Heroes"),
    FString("Heroes tags")
  );

  GameplayTags.Heroes_Aura = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Heroes.Aura"),
    FString("Aura tag")
  );

  GameplayTags.Heroes_Vilkar = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Heroes.Vilkar"),
    FString("Vilkar tag")
  );
  
  /*
   * DIFFICULTY CLASSES
   */

  GameplayTags.DifficultyClass = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass"),
    FString("Difficulty Class tags")
  );

  GameplayTags.DifficultyClass_Initial = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Initial"),
    FString("Initial areas difficulty")
  );

  GameplayTags.DifficultyClass_Easy = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Easy"),
    FString("Easy difficulty")
  );

  GameplayTags.DifficultyClass_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Normal"),
    FString("Normal difficulty")
  );
  GameplayTags.DifficultyClass_Hard = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Hard"),
    FString("Hard difficulty")
  );
  GameplayTags.DifficultyClass_Extreme = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Extreme"),
    FString("Extreme difficulty")
  );
  GameplayTags.DifficultyClass_Special = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("DifficultyClass.Special"),
    FString("Special combats")
  );

  /*
   * PRIMARY ATTRIBUTES
   */

  GameplayTags.Attributes = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes"),
    FString("Attribute tags")
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
   * PRIMARY ATTRIBUTES
   */

  GameplayTags.Attributes_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary"),
    FString("Primary attribute tags")
  );
  
  GameplayTags.Attributes_Primary_Ferocity = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Ferocity"),
    FString("Ferocity attribute tag")
  );

  GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Agility"),
    FString("Agility attribute tag")
  );

  GameplayTags.Attributes_Primary_Toughness = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Toughness"),
    FString("Toughness attribute tag")
  );

  GameplayTags.Attributes_Primary_Attunement = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Attunement"),
    FString("Attunement attribute tag")
  );

  GameplayTags.Attributes_Primary_Willpower = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Willpower"),
    FString("Willpower attribute tag")
  );

  GameplayTags.Attributes_Primary_Faith = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Primary.Faith"),
    FString("Faith attribute tag")
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

  GameplayTags.Attributes_Secondary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.AttackSpeed"),
    FString("Attack speed")
  );

  GameplayTags.Attributes_Secondary_CastSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.CastSpeed"),
    FString("Cast speed")
  );

  GameplayTags.Attributes_Secondary_MovementSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.MovementSpeed"),
    FString("Movement speed")
  );

  GameplayTags.Attributes_Secondary_CooldownSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.CooldownSpeed"),
    FString("Cooldown recovery speed")
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
    FString("Amount of health recovered after each fight")
  );

  GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get()
    .AddNativeGameplayTag(
      FName("Attributes.Secondary.ManaRegeneration"),
      FString("Amount of mana recovered over time")
    );

  GameplayTags.Attributes_Secondary_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.Dodge"),
    FString("Chance to dodge a hit attack and negate damage")
  );

  GameplayTags.Attributes_Secondary_Parry = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Secondary.Parry"),
    FString("Increases Parry window, health recovered and counter attack damage")
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

  GameplayTags.Damage_Chaos = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Chaos"),
    FString("Chaos damage type")
  );
  
  GameplayTags.Damage_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Air"),
    FString("Air damage type")
  );
  
  GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Fire"),
    FString("Fire damage type")
  );

  GameplayTags.Damage_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Water"),
    FString("Water damage type")
  );

  GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Lightning"),
    FString("Lightning damage type")
  );

  GameplayTags.Damage_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Earth"),
    FString("Earth damage type")
  );

  /*
   * Heal Tags
   */

  GameplayTags.Heal = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Heal"),
    FString("Heal tag")
  );

  /*
   * State Tags
   */

  GameplayTags.State = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("State"),
    FString("State tags")
  );

  GameplayTags.State_Default = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("State.Default"),
    FString("Default state tag")
  );

  GameplayTags.State_InCombat = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("State.InCombat"),
    FString("In combat state tag")
  );


  /*
   * Resistances Tags
   */

  GameplayTags.Attributes_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance"),
    FString("Resistance attribute tags")
  );

  GameplayTags.Attributes_Resistance_Chaos = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Energy"),
    FString("Resistance to Energy damage")
  );
  
  GameplayTags.Attributes_Resistance_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Air"),
    FString("Resistance to Air damage")
  );
  
  GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Fire"),
    FString("Resistance to Fire damage")
  );

  GameplayTags.Attributes_Resistance_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Water"),
    FString("Resistance to Water damage")
  );

  GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Lightning"),
    FString("Resistance to Lightning damage")
  );

  GameplayTags.Attributes_Resistance_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Earth"),
    FString("Resistance to Earth damage")
  );

  /*
   * Map of Damage Types to Resistances
   */

  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Chaos,
    GameplayTags.Attributes_Resistance_Chaos
  );
  
  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Fire,
    GameplayTags.Attributes_Resistance_Fire
  );

  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Water,
    GameplayTags.Attributes_Resistance_Water
  );
  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Air,
    GameplayTags.Attributes_Resistance_Air
  );

  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Earth,
    GameplayTags.Attributes_Resistance_Earth
  );
  
  GameplayTags.DamageTypesToResistances.Add(
    GameplayTags.Damage_Lightning,
    GameplayTags.Attributes_Resistance_Lightning
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

  GameplayTags.StatusEffects_Stacks = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Stacks"),
    FString("Stacks of the the status effect applied in a single application")
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

  GameplayTags.StatusEffects_Buff_Haste = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Buff.Haste"),
    FString("Increases action speed")
  );

  GameplayTags.StatusEffects_Buff_Clarity = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Buff.Clarity"),
    FString("The next ability commited doesn't cost mana")
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

  GameplayTags.StatusEffects_Debuff_Weakness = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Debuff.Weakness"),
    FString("Weakness effect. Reduces damage done.")
  );

  GameplayTags.StatusEffects_Debuff_Exposed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Debuff.Exposed"),
    FString("Exposed effect. Increases damage received.")
  );

  GameplayTags.StatusEffects_Debuff_Root = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Debuff.Root"),
    FString("Root effect. Can't move or use mobility abilities.")
  );

  GameplayTags.StatusEffects_Debuff_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Debuff.Shock"),
    FString("Shock effect. Gets paralyzed every 1.5s.")
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
    FString(
      "Freeze effect. Inflicted when having many stacks of Chill applied. Can't do anything. Temporarily resist new applications after recovery."
    )
  );

  GameplayTags.StatusEffects_Incapacitation_Sleep = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Incapacitation.Sleep"),
    FString("Sleep effect. Can't do anything. Removed when taking instant damage.")
  );

  GameplayTags.StatusEffects_Incapacitation_Paralysis = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("StatusEffects.Incapacitation.Paralysis"),
    FString("Paralysis effect. Can't do anything.")
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

  GameplayTags.Immunities_Force = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Immunities.Force"),
    FString("Immunity to any force effects (pull, knockback...)")
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

  // Movement

  GameplayTags.Abilities_Mobility = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Mobility"),
    FString("Mobility ability.")
  );

  // Health Regen

  GameplayTags.Abilities_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.HealthRegeneration"),
    FString("HealthRegeneration ability.")
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

  GameplayTags.Abilities_Status = UGameplayTagsManager::Get().AddNativeGameplayTag(
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

  GameplayTags.Abilities_Element_Chaos = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Chaos"),
    FString("Chaos element ability tag.")
  );
  

  GameplayTags.Abilities_Element_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Fire"),
    FString("Fire element ability tag.")
  );

  GameplayTags.Abilities_Element_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Water"),
    FString("Water element ability tag.")
  );

  GameplayTags.Abilities_Element_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Air"),
    FString("Air element ability tag.")
  );
  
  GameplayTags.Abilities_Element_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Earth"),
    FString("Earth element ability tag.")
  );
  
  GameplayTags.Abilities_Element_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Element.Lightning"),
    FString("Lightning element ability tag.")
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

  GameplayTags.Abilities_Reaction_KnockbackEnd = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.KnockbackEnd"),
    FString("End knockback effect ability")
  );

  GameplayTags.Abilities_Reaction_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.Stun"),
    FString("Get stunned ability")
  );

  GameplayTags.Abilities_Reaction_Charm = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.Charm"),
    FString("Get charmed ability")
  );

  GameplayTags.Abilities_Reaction_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.Shock"),
    FString("Get shocked ability")
  );

  GameplayTags.Abilities_Reaction_Paralysis = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.Paralysis"),
    FString("Get paralyzed ability")
  );


  GameplayTags.Abilities_Reaction_ShieldStackRemove = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.ShieldStackRemove"),
    FString("Removes stacks of shield if applicable")
  );

  GameplayTags.Abilities_Reaction_SecondWind = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Reaction.SecondWind"),
    FString("Activated on death if character has at least 1 stack")
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

  // Interactions

  GameplayTags.Abilities_Interaction = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Interaction"),
    FString("Interaction ability tags")
  );

  GameplayTags.Abilities_Interaction_SoulEssence = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Interaction.SoulEssence"),
    FString("SoulEssence interaction ability")
  );

  GameplayTags.Abilities_Interaction_ElementalEssence = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Interaction.ElementalEssence"),
    FString("ElementalEssence interaction ability")
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

  GameplayTags.Abilities_Active_Aura_FireMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.FireMissiles"),
    FString("FireMissiles ability")
  );

  GameplayTags.Cooldown_Active_Aura_FireMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.FireMissiles"),
    FString("FireMissiles ability cooldown")
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

  GameplayTags.Abilities_Active_Aura_LightningMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.LightningMissiles"),
    FString("LightningMissiles ability")
  );

  GameplayTags.Cooldown_Active_Aura_LightningMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.LightningMissiles"),
    FString("LightningMissiles ability cooldown")
  );

  // Aura Water Abilities

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

  GameplayTags.Abilities_Active_Aura_IceMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.IceMissiles"),
    FString("IceMissiles ability")
  );

  GameplayTags.Cooldown_Active_Aura_IceMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.IceMissiles"),
    FString("IceMissiles ability cooldown")
  );

  GameplayTags.Charges_Active_Aura_IceMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Charges.Active.Aura.IceMissiles"),
    FString("IceMissiles ability cooldown")
  );

  // Aura Chaos Abilities

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

  // Aura Earth Abilities

  GameplayTags.Abilities_Active_Aura_NecroBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.NecroBolt"),
    FString("NecroBolt ability")
  );

  GameplayTags.Cooldown_Active_Aura_NecroBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.NecroBolt"),
    FString("NecroBolt ability cooldown")
  );

  GameplayTags.Abilities_Active_Aura_NecroMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.NecroMissiles"),
    FString("NecroMissiles ability")
  );

  GameplayTags.Cooldown_Active_Aura_NecroMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.NecroMissiles"),
    FString("NecroMissiles ability cooldown")
  );

  // Aura Air Abilities

  GameplayTags.Abilities_Active_Aura_WindGust = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.WindGust"),
    FString("WindGust ability")
  );

  GameplayTags.Cooldown_Active_Aura_WindGust = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.WindGust"),
    FString("WindGust ability cooldown")
  );

  GameplayTags.Abilities_Active_Aura_RockMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Active.Aura.RockMissiles"),
    FString("RockMissiles ability")
  );

  GameplayTags.Cooldown_Active_Aura_RockMissiles = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Active.Aura.RockMissiles"),
    FString("RockMissiles ability cooldown")
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

  GameplayTags.Abilities_Passive_Common = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Common"),
    FString("Common passive abilities")
  );

  GameplayTags.Abilities_Passive_Upgrade = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade"),
    FString("Upgrade passive abilities")
  );

  // Soul upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_SoulAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.SoulAbsorption"),
    FString("SoulAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_SoulBalance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.SoulBalance"),
    FString("SoulBalance ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_WildSoul = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.WildSoul"),
    FString("WildSoul ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_Respite = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Respite"),
    FString("Respite ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_ShortRest = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.ShortRest"),
    FString("ShortRest ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_Brilliance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Brilliance"),
    FString("Brilliance ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_Clarity = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Clarity"),
    FString("Clarity ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_SecondWind = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.SecondWind"),
    FString("SecondWind ability")
  );

  // Fire upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_FireAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.FireAbsorption"),
    FString("FireAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_FireControl = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.FireControl"),
    FString("FireControl ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_Pyromania = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Pyromania"),
    FString("Pyromania ability")
  );

  // Water upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_IceAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.IceAbsorption"),
    FString("IceAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_IceControl = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.IceControl"),
    FString("IceControl ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_Cryogenesis = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Cryogenesis"),
    FString("Cryogenesis ability")
  );

  // Lightning upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_LightningAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.LightningAbsorption"),
    FString("LightningAbsorption ability")
  );
  
  GameplayTags.Abilities_Passive_Upgrade_LightningControl = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.LightningControl"),
    FString("LightningControl ability")
  );
  
  GameplayTags.Abilities_Passive_Upgrade_Stormbound = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.Stormbound"),
    FString("Stormbound ability")
  );

  // Earth upgrade abilities
  
  GameplayTags.Abilities_Passive_Upgrade_NecroticAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.NecroticAbsorption"),
    FString("NecroticAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_NecroticControl = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.NecroticControl"),
    FString("NecroticControl ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_FeedOnTheWeak = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.FeedOnTheWeak"),
    FString("FeedOnTheWeak ability")
  );

  // Chaos upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_ArcaneAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.ArcaneAbsorption"),
    FString("ArcaneAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_ArcaneControl = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.ArcaneControl"),
    FString("ArcaneControl ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_MysticSurge = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.MysticSurge"),
    FString("MysticSurge ability")
  );

  // Air upgrade abilities

  GameplayTags.Abilities_Passive_Upgrade_PhysicalAbsorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.PhysicalAbsorption"),
    FString("PhysicalAbsorption ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_PhysicalBalance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.PhysicalBalance"),
    FString("PhysicalBalance ability")
  );

  GameplayTags.Abilities_Passive_Upgrade_BrawlStar = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.Upgrade.BrawlStar"),
    FString("BrawlStar ability")
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

  // Ability Events

  GameplayTags.Abilities_Event = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Event"),
    FString("Ability events tags")
  );

  GameplayTags.Abilities_Event_SecondWindTrigger = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Event.SecondWindTrigger"),
    FString("SecondWind trigger event")
  );

  /*
   * Player Upgrades
   */

  GameplayTags.Upgrades = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades"),
    FString("Upgrades tags")
  );

  // Soul

  GameplayTags.Upgrades_Soul = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul"),
    FString("Soul upgrades tags")
  );

  GameplayTags.Upgrades_Soul_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Absorption"),
    FString("Soul Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Soul_Balance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Balance"),
    FString("Soul Balance upgrade tag")
  );

  GameplayTags.Upgrades_Soul_Wild = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Wild"),
    FString("Wild Soul upgrade tag")
  );

  GameplayTags.Upgrades_Soul_Respite = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Respite"),
    FString("Respite upgrade tag")
  );

  GameplayTags.Upgrades_Soul_ShortRest = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.ShortRest"),
    FString("Short Rest upgrade tag")
  );

  GameplayTags.Upgrades_Soul_Brilliance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Brilliance"),
    FString("Brilliance upgrade tag")
  );

  GameplayTags.Upgrades_Soul_Clarity = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.Clarity"),
    FString("Clarity upgrade tag")
  );

  GameplayTags.Upgrades_Soul_SecondWind = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Soul.SecondWind"),
    FString("SecondWind upgrade tag")
  );

  // Fire

  GameplayTags.Upgrades_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Fire"),
    FString("Fire upgrades tags")
  );

  GameplayTags.Upgrades_Fire_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Fire.Absorption"),
    FString("Fire Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Fire_Control = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Fire.Control"),
    FString("Fire Control upgrade tag")
  );

  GameplayTags.Upgrades_Fire_Pyromania = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Fire.Pyromania"),
    FString("Pyromania upgrade tag")
  );

  // Water

  GameplayTags.Upgrades_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ice"),
    FString("Ice upgrades tags")
  );

  GameplayTags.Upgrades_Ice_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ice.Absorption"),
    FString("Ice Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Ice_Control = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ice.Control"),
    FString("Ice Control upgrade tag")
  );

  GameplayTags.Upgrades_Ice_Cryogenesis = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ice.Cryogenesis"),
    FString("Cryogenesis upgrade tag")
  );

  // Lightning

  GameplayTags.Upgrades_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Lightning"),
    FString("Lightning upgrades tags")
  );

  GameplayTags.Upgrades_Lightning_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Lightning.Absorption"),
    FString("Lightning Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Lightning_Control = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Lightning.Control"),
    FString("Lightning Control upgrade tag")
  );

  GameplayTags.Upgrades_Lightning_Stormbound = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Lightning.Stormbound"),
    FString("Stormbound upgrade tag")
  );

  // Earth

  GameplayTags.Upgrades_Necrotic = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Necrotic"),
    FString("Necrotic upgrades tags")
  );

  GameplayTags.Upgrades_Necrotic_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Necrotic.Absorption"),
    FString("Necrotic Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Necrotic_Control = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Necrotic.Control"),
    FString("Necrotic Control upgrade tag")
  );

  GameplayTags.Upgrades_Necrotic_FeedOnTheWeak = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Necrotic.FeedOnTheWeak"),
    FString("FeedOnTheWeak upgrade tag")
  );

  // Chaos

  GameplayTags.Upgrades_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Arcane"),
    FString("Arcane upgrades tags")
  );

  GameplayTags.Upgrades_Arcane_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Arcane.Absorption"),
    FString("Arcane Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Arcane_Control = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Arcane.Control"),
    FString("Arcane Control upgrade tag")
  );

  GameplayTags.Upgrades_Arcane_MysticSurge = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Arcane.MysticSurge"),
    FString("MysticSurge upgrade tag")
  );

  // Air

  GameplayTags.Upgrades_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Physical"),
    FString("Physical upgrades tags")
  );

  GameplayTags.Upgrades_Physical_Absorption = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Physical.Absorption"),
    FString("Physical Absorption upgrade tag")
  );

  GameplayTags.Upgrades_Physical_Balance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Physical.Balance"),
    FString("Physical Balance upgrade tag")
  );

  GameplayTags.Upgrades_Physical_BrawlStar = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Physical.BrawlStar"),
    FString("BrawlStar upgrade tag")
  );

  // Abilities Upgrades

  GameplayTags.Upgrades_Ability_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ability.Dodge"),
    FString("Dodge ability upgrade tag")
  );

  GameplayTags.Upgrades_Ability_Dodge_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ability.Dodge.Dash"),
    FString("Dodge Dash upgrade tag")
  );

  GameplayTags.Upgrades_Ability_Dodge_Rush = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ability.Dodge.Rush"),
    FString("Dodge Rush upgrade tag")
  );

  GameplayTags.Upgrades_Ability_Dodge_Boost = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ability.Dodge.Boost"),
    FString("Dodge Boost upgrade tag")
  );

  GameplayTags.Upgrades_Ability_Dodge_Maneuver = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Upgrades.Ability.Dodge.Maneuver"),
    FString("Dodge Maneuver upgrade tag")
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

  GameplayTags.Resources_Essence_Air = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Air"),
    FString("Air essence resource")
  );

  GameplayTags.Resources_Essence_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Fire"),
    FString("Fire essence resource")
  );

  GameplayTags.Resources_Essence_Water = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Water"),
    FString("Water essence resource")
  );

  GameplayTags.Resources_Essence_Earth = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Earth"),
    FString("Earth essence resource")
  );

  GameplayTags.Resources_Essence_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Lightning"),
    FString("Lightning essence resource")
  );

  GameplayTags.Resources_Essence_Chaos = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Resources.Essence.Chaos"),
    FString("Chaos essence resource")
  );

  /*
   * MODIFIERS
   */

  GameplayTags.Modifiers = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers"),
    FString("Modifiers")
  );

  GameplayTags.Modifiers_Fool = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Fool"),
    FString("The Fool modifier. Cost and stats reduced to 50%.")
  );

  GameplayTags.Modifiers_Magician = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Magician"),
    FString("The Magician modifier. Cost and stats increased to 200%.")
  );

  GameplayTags.Modifiers_HighPriestess = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.HighPriestess"),
    FString("The High Priestess modifier. Heal each time the skill is used.")
  );
  
  GameplayTags.Modifiers_Empress = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Empress"),
    FString("The Empress modifier. Skill have +1 burst. Mana is still consumed.")
  );
  
  GameplayTags.Modifiers_Emperor = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Emperor"),
    FString("The Emperor modifier. Double amount of Charges.")
  );

  GameplayTags.Modifiers_Hierophant = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Hierophant"),
    FString("The Hierophant modifier.")
  );

  GameplayTags.Modifiers_Lovers = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Lovers"),
    FString("The Lovers modifier.")
  );

  GameplayTags.Modifiers_Chariot = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Chariot"),
    FString("The Chariot modifier. Added or increased knockback force.")
  );
  
  GameplayTags.Modifiers_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Strength"),
    FString("The Strength modifier. Damage shifts from -25% to +50%.")
  );

  GameplayTags.Modifiers_Hermit = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Hermit"),
    FString("The Hermit modifier. All Charges and Projectiles amount are set to 1. Stats increased accordingly.")
  );

  GameplayTags.Modifiers_Fortune = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Fortune"),
    FString("The Fortune modifier. Increased critical hit chance.")
  );
  
  GameplayTags.Modifiers_Justice = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Justice"),
    FString("The Justice modifier. Critical hits heal the caster.")
  );

  GameplayTags.Modifiers_HangedMan = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.HangedMan"),
    FString("The HangedMan modifier. Has 5 charges that doesn't recover. Stats increased by 100%.")
  );

  GameplayTags.Modifiers_Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Death"),
    FString("The Death modifier. Once dying while holding this ability, it will transform with another one with another modifier.")
  );

  GameplayTags.Modifiers_Temperance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Temperance"),
    FString("The Temperance modifier. If not used for a certain time, it goes on cooldown on its own. Mana cost is removed.")
  );

  GameplayTags.Modifiers_Devil = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Devil"),
    FString("The Devil modifier. Other skills can only be used when this one is on cooldown. Cooldown and mana cost reduced to 50%")
  );

  GameplayTags.Modifiers_Tower = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Tower"),
    FString("The Tower modifier. Stats reduced by 30%. All other skills stats increased by 15%")
  );

  GameplayTags.Modifiers_Star = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Star"),
    FString("The Star modifier. While on cooldown, mana recovery increased by 100%.")
  );

  GameplayTags.Modifiers_Moon = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Moon"),
    FString("The Moon modifier. Takes extra 20% damage when not on cooldown. Takes -25% damage when on cooldown.")
  );
  
  GameplayTags.Modifiers_Sun = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Sun"),
    FString("The Sun modifier. Heal for 10% mana cost when used.")
  );

  GameplayTags.Modifiers_Judgement = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.Judgement"),
    FString("The Judgement modifier. Ignores resistances.")
  );

  GameplayTags.Modifiers_World = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Modifiers.World"),
    FString("The World modifier. Stats increased by 25%, mana cost reduced by 50%.")
  );

  /*
   * Map of Tags to their children
   */

  const TArray SkillsInputTags(
    {
      GameplayTags.InputTag_1,
      GameplayTags.InputTag_2,
      GameplayTags.InputTag_3,
      GameplayTags.InputTag_4
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.InputTag, SkillsInputTags);

  const TArray BuffTags(
    {
      GameplayTags.StatusEffects_Buff_Shield,
      GameplayTags.StatusEffects_Buff_Haste
    }
  );
  const TArray DebuffTags(
    {
      GameplayTags.StatusEffects_Debuff_Bleeding,
      GameplayTags.StatusEffects_Debuff_Burning,
      GameplayTags.StatusEffects_Debuff_Chill,
      GameplayTags.StatusEffects_Debuff_Charm,
      GameplayTags.StatusEffects_Debuff_Weakness,
      GameplayTags.StatusEffects_Debuff_Exposed,
      GameplayTags.StatusEffects_Debuff_Root,
      GameplayTags.StatusEffects_Debuff_Shock,
    }
  );
  const TArray IncapacitationTags(
    {
      GameplayTags.StatusEffects_Incapacitation_HitReact,
      GameplayTags.StatusEffects_Incapacitation_Knockback,
      GameplayTags.StatusEffects_Incapacitation_Stun,
      GameplayTags.StatusEffects_Incapacitation_Freeze,
      GameplayTags.StatusEffects_Incapacitation_Sleep,
      GameplayTags.StatusEffects_Incapacitation_Paralysis
    }
  );
  const TArray ConditionTags(
    {
      GameplayTags.StatusEffects_Condition_Alive,
      GameplayTags.StatusEffects_Condition_Dead,
      GameplayTags.StatusEffects_Condition_Reanimated,
      GameplayTags.StatusEffects_Condition_Possessed,
    }
  );
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

  const TArray ElementTags(
    {
      GameplayTags.Abilities_Element_Air,
      GameplayTags.Abilities_Element_Chaos,
      GameplayTags.Abilities_Element_Fire,
      GameplayTags.Abilities_Element_Water,
      GameplayTags.Abilities_Element_Lightning,
      GameplayTags.Abilities_Element_Earth,
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.Abilities_Element, ElementTags);

  const TArray TierTags(
    {
      GameplayTags.Abilities_Tier_I,
      GameplayTags.Abilities_Tier_II,
      GameplayTags.Abilities_Tier_III,
      GameplayTags.Abilities_Tier_IV,
      GameplayTags.Abilities_Tier_V,
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.Abilities_Tier, TierTags);

  const TArray EssenceTags = TArray(
    {
      GameplayTags.Resources_Essence_Soul
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.Resources_Essence, EssenceTags);

  const TArray PrimaryAttributeTags = TArray(
    {
      GameplayTags.Attributes_Primary_Ferocity,
      GameplayTags.Attributes_Primary_Agility,
      GameplayTags.Attributes_Primary_Toughness,
      GameplayTags.Attributes_Primary_Attunement,
      GameplayTags.Attributes_Primary_Willpower,
      GameplayTags.Attributes_Primary_Faith,
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.Attributes_Primary, PrimaryAttributeTags);

  const TArray ModifierTags = TArray(
    {
      GameplayTags.Modifiers_Fool,
      GameplayTags.Modifiers_Magician,
      GameplayTags.Modifiers_HighPriestess,
      GameplayTags.Modifiers_Empress,
      GameplayTags.Modifiers_Emperor,
      GameplayTags.Modifiers_Hierophant,
      GameplayTags.Modifiers_Lovers,
      GameplayTags.Modifiers_Chariot,
      GameplayTags.Modifiers_Strength,
      GameplayTags.Modifiers_Hermit,
      GameplayTags.Modifiers_Fortune,
      GameplayTags.Modifiers_Justice,
      GameplayTags.Modifiers_HangedMan,
      GameplayTags.Modifiers_Death,
      GameplayTags.Modifiers_Temperance,
      GameplayTags.Modifiers_Devil,
      GameplayTags.Modifiers_Tower,
      GameplayTags.Modifiers_Star,
      GameplayTags.Modifiers_Moon,
      GameplayTags.Modifiers_Sun,
      GameplayTags.Modifiers_Judgement,
      GameplayTags.Modifiers_World
    }
  );

  GameplayTags.ParentsToChildren.Add(GameplayTags.Modifiers, ModifierTags);
  
  GameplayTags.bIsValid = true;
}
