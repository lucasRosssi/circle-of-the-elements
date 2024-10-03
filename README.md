# Welcome to Aura! 👋

Aura is a Top-Down roguelite, with some RPG mechanics and revolves around different elements. The player can learn new element abilities through their run, collect elemental essences and permanently upgrade their character using the available resources.

## Project Structure

## Classes

### Abilities

#### BaseAbility
It is the base gameplay ability class used in the project, almost all other ability classes derive from it.


#### ActiveAbility
It is the base gameplay ability class that needs an input to be activated.


#### PassiveAbility
It is the base gameplay ability class that are always active on its own, as soon as the character acquires it.


#### ActiveDamageAbility
It is the base gameplay ability class that applies damage.


#### ProjectileAbility
Long ranged abilities that launches a projectile of the class AuraProjectile, and the projectile is responsible for applying the ability effects on targets it hits.


#### MeleeAttackAbility
Close ranged abilities that applies its effects on close targets. Has different trace modes, such as Sphere, Box, Cone, WeaponBox and Line.


#### BeamAbility
Long ranged abilities that uses line trace to project beam-type particle effects and applies its effects on the first target hit by the trace.


#### AreaAbility
Long ranged area abilities that first uses a targeting state and then perform its effects directly in the selected targeting location. Usually applies effects instantly.


#### AreaEffectAbility
Longe ranged area abilities that first uses a targeting state and then spawns an actor of the class AreaEffectActor in the selected targeting location. Usually applies effects periodically in the area, but can also apply instantly. The area "active" state can be delayed after selecting the targeting location.


### Components

#### InteractComponent
#### TeamComponent

### Managers

#### EncounterManager
#### LocationManager
#### RewardManager
#### AbilityManager
#### UIManager

## Blueprints
