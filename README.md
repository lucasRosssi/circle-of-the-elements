# Welcome to Circle of the Elements! 👋

Circle of the Elements is a Top Down RPG with a druid protagonist that fights goblins to free the elemental spirits they imprisioned in jewels. The freed spirits join the druid by providing new elemental powers.

## Tech Stack

### Gameplay Ability System
GAS has a huge part in this project. It is involved in the character skills, attributes, status effects, object interactions and progression. There is a variety of abilities such as melee, projectiles, beam (trace), area and passive. The most difficult part of it is its setup as it requires a lot of boilerplate work, but as soon as it is ready it is really scalable and allowed me to easily expand it with a lot of new mechanics, such as a melee combo system and ability charges.

#### Melee Abilities

https://github.com/user-attachments/assets/fbb60be8-5ba2-464f-a0b8-b87077576a86

#### Projectile Abilities

https://github.com/user-attachments/assets/d3462069-abce-4c89-87d0-732138a193ce

#### Beam Abilities

https://github.com/user-attachments/assets/f2bcd2dd-485a-471a-8b7b-60782731c6fa

#### Area Abilities
1) Instant
   
https://github.com/user-attachments/assets/cfdeb5d6-37f9-4614-a934-8d8d8aa08598

2) Periodic

https://github.com/user-attachments/assets/edc9fa4b-e461-4415-8b00-458b2aa9c6cc

### UI
The project uses the MVC pattern with widget controller classes that work as links between the ability system information and the widgets. Abilities descriptions, icons, properties, cooldowns, charges and other data are stored in data assets and organized with Gameplay Tags that maps abilities, attributes and status effects.

### Components
#### Team Component
Handles team assignment and integrates with the ability system by validating ability targets (allies, enemies). Summon abilities and mental status effects (such as Charm) also interact with this component.

#### Status Effects Manager Component
A single component that handles all status effects visual effects. Spawns the corresponding niagara based on the gameplay effect applied (based on the tags).

#### Orbit Manager Component
Component that handles orbiting equipped spirits, positioning them in their corresponding ability socket when being used, hiding/showing with cooldown and scaling with charges.

1) Orbiting

https://github.com/user-attachments/assets/fde2126d-de62-48bd-ba81-1338a7ea3d22

2) Attaching and hiding

https://github.com/user-attachments/assets/f301a166-66d7-458f-b86a-dc420d025b62

3) Scaling

https://github.com/user-attachments/assets/d5ab295b-a6db-41c1-8e89-05aefb753e63

#### Game Mode Managers
Components that handle gameplay logic, such as combat encounters, enemy spawn, loot randomness (weighted probability, shuffle bag). 
