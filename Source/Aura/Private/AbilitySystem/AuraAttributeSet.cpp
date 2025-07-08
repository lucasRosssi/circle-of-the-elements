// Copyright Lucas Rossi


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Enums/CharacterType.h"
#include "GameFramework/Character.h"
#include "Interfaces/AttributeSetInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Utils/AuraSystemsLibrary.h"

UAuraAttributeSet::UAuraAttributeSet()
{
  InitHealth(GetMaxHealth());
  InitMana(GetMaxMana());
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(
  TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  // Primary
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Ferocity, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Agility, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Attunement, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Willpower, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Faith, COND_None, REPNOTIFY_Always);

  // Secondary
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CastSpeed, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CooldownSpeed, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalRate, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Dodge, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Parry, COND_None, REPNOTIFY_Always);

  // Vital
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

  // Resistance
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ChaosResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, WaterResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, AirResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, EarthResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);

  // Special
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Power, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, TimeDilation, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(
  const FGameplayAttribute& Attribute,
  float& NewValue
)
{
  Super::PreAttributeChange(Attribute, NewValue);
}

void UAuraAttributeSet::PreAttributeBaseChange(
  const FGameplayAttribute& Attribute, float&
  NewValue
) const
{
  Super::PreAttributeBaseChange(Attribute, NewValue);

  if (Attribute == GetHealthAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
  }

  if (Attribute == GetManaAttribute())
  {
    NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
  }

  if (Attribute == GetIncomingDamageAttribute())
  {
    if (bAvatarDead)
    {
      NewValue = 0.f;
    }
  }
}

void UAuraAttributeSet::HandleDeath(
  const FEffectProperties& Props,
  float DeathImpulseMagnitude
)
{
  bAvatarDead = true;
  SendXPEvent(Props);

  if (GetAvatarCombatInterface())
  {
    const FVector ForwardVector =
      UAuraAbilitySystemLibrary::GetForwardVector(Props.EffectContextHandle);

    AvatarCombatInterface->Die(ForwardVector * DeathImpulseMagnitude);
  }

  if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
  {
    ASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer({
      FAuraGameplayTags::Get().StatusEffects
    }));
  }
}

void UAuraAttributeSet::PostAttributeChange(
  const FGameplayAttribute& Attribute,
  float OldValue,
  float NewValue
)
{
  Super::PostAttributeChange(Attribute, OldValue, NewValue);

  if (Attribute == GetMaxHealthAttribute())
  {
    const float Amount = NewValue - OldValue;
    if (Amount > 0)
    {
      SetHealth(GetHealth() + Amount);
    }
    if (Amount < 0)
    {
      SetHealth(FMath::Min(GetHealth(), GetMaxHealth()));
    }
  }

  if (Attribute == GetMaxManaAttribute())
  {
    const float Amount = NewValue - OldValue;
    if (Amount > 0)
    {
      SetMana(GetMana() + Amount);
    }
    if (Amount < 0)
    {
      SetMana(FMath::Min(GetMana(), GetMaxMana()));
    }
  }
    
  if (Attribute == GetMovementSpeedAttribute())
  {
    IAttributeSetInterface::Execute_SetMovementSpeed(GetOwningActor(), NewValue);
  }

  if (Attribute == GetTimeDilationAttribute())
  {
    IAttributeSetInterface::Execute_SetTimeDilation(GetOwningActor(), NewValue);
  }
}

void UAuraAttributeSet::SetEffectProperties(
  const FGameplayEffectModCallbackData& Data,
  FEffectProperties& Props
) const
{
  // Source = causer of the effect, Target = target of the effect (owner of this AS)

  Props.EffectContextHandle = Data.EffectSpec.GetContext();
  Props.SourceASC = Props.EffectContextHandle
                         .GetOriginalInstigatorAbilitySystemComponent();

  if (
    IsValid(Props.SourceASC) &&
    Props.SourceASC->AbilityActorInfo.IsValid() &&
    Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid()
  )
  {
    Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
    Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
    if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
    {
      if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
      {
        Props.SourceController = Pawn->GetController();
      }
    }
    if (Props.SourceController)
    {
      Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
    }
  }

  if (
    Data.Target.AbilityActorInfo.IsValid() &&
    Data.Target.AbilityActorInfo->AvatarActor.IsValid()
  )
  {
    Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
    Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
    Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
    Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
  }
}


void UAuraAttributeSet::HandleIncomingDamage(
  const FEffectProperties& Props,
  const FGameplayAttribute& Attribute
)
{
  if (GetIncomingDamage() + GetIncomingDoT() <= 0.f) return;

  const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

  const bool bDoT = Attribute == GetIncomingDoTAttribute();
  float LocalIncomingDamage;
  bool bParried;
  bool bCriticalHit;

  if (bDoT)
  {
    LocalIncomingDamage = GetIncomingDoT();
    SetIncomingDoT(0);
    bParried = false;
    bCriticalHit = false;
  }
  else
  {
    LocalIncomingDamage = GetIncomingDamage();
    SetIncomingDamage(0.f);
    bParried = UAuraAbilitySystemLibrary::IsParried(Props.EffectContextHandle);
    bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
  }

  const float NewHealth = GetHealth() - LocalIncomingDamage;
  SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

  const bool bFatal = NewHealth <= 0.f;
  if (bFatal)
  {
    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ASC->HasMatchingGameplayTag(AuraTags.Upgrades_Soul_SecondWind))
    {
      const FGameplayEffectQuery& DebuffsQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
        FGameplayTagContainer({ AuraTags.StatusEffects_Debuff })
        );
      ASC->RemoveActiveEffects(DebuffsQuery, -1);
      
      const FGameplayEffectQuery& SecondWindQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(
        FGameplayTagContainer({ AuraTags.Upgrades_Soul_SecondWind })
        );
      ASC->RemoveActiveEffects(SecondWindQuery, 1);

      FGameplayEventData EventData;
      EventData.Instigator = Props.SourceAvatarActor;
      EventData.Target = Props.TargetAvatarActor;
      EventData.ContextHandle = Props.EffectContextHandle;
      Props.SourceASC->GetOwnedGameplayTags(EventData.InstigatorTags);
      Props.TargetASC->GetOwnedGameplayTags(EventData.TargetTags);
      ASC->HandleGameplayEvent(AuraTags.Abilities_Event_SecondWindTrigger, &EventData);
    }
    else
    {
      const float DeathImpulseMagnitude = bDoT ? 0.f : FMath::Min(250 * LocalIncomingDamage, 20000.f);
      HandleDeath(Props, DeathImpulseMagnitude);
    }
    
  }
  else if (!bDoT && UAuraAbilitySystemLibrary::GetApplyHitReact(Props.EffectContextHandle))
  {
    const FGameplayTagContainer TagContainer({
      FAuraGameplayTags::Get().Abilities_Reaction_HitReact
    });
    Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
  }

  ShowDamageFloatingText(Props, LocalIncomingDamage, bParried, bCriticalHit);
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
  const float LocalIncomingXP = GetIncomingXP();
  SetIncomingXP(0.f);

  IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
}

void UAuraAttributeSet::HandleKnockback(const FEffectProperties& Props)
{
  const float LocalIncomingForce = GetIncomingForce();
  
  if (FMath::IsNearlyZero(LocalIncomingForce)) return;
  
  SetIncomingForce(0.f);

  if (!Props.TargetCharacter->Implements<UCombatInterface>()) return;
  
  FVector ForwardVector =
    UAuraAbilitySystemLibrary::GetForwardVector(Props.EffectContextHandle);
  ForwardVector.Z = 0.f;
  const FVector KnockbackForce = ForwardVector * LocalIncomingForce;

  ICombatInterface::Execute_ApplyKnockback(Props.TargetCharacter, KnockbackForce);
}

void UAuraAttributeSet::HandleHeal(const FEffectProperties& Props)
{
  if (GetIncomingHeal() < 1.f) return;
    
  const float LocalIncomingHeal = GetIncomingHeal();
    
  SetIncomingHeal(0.f);
    
  const float NewHealth = GetHealth() + LocalIncomingHeal;
  SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

  ShowHealFloatingText(Props, LocalIncomingHeal);
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
  Super::PostGameplayEffectExecute(Data);

  FEffectProperties Props;
  SetEffectProperties(Data, Props);

  if (bAvatarDead) return;
  if (!IsValid(Props.SourceCharacter)) return;

  if (
    Data.EvaluatedData.Attribute == GetIncomingDamageAttribute() ||
    Data.EvaluatedData.Attribute == GetIncomingDoTAttribute()
  )
  {
    HandleIncomingDamage(Props, Data.EvaluatedData.Attribute);
  }

  if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
  {
    HandleIncomingXP(Props);
  }

  if (Data.EvaluatedData.Attribute == GetIncomingForceAttribute())
  {
    HandleKnockback(Props);
  }

  if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
  {
    HandleHeal(Props);
  }
}

void UAuraAttributeSet::ShowDamageFloatingText(
  const FEffectProperties& Props,
  float Damage,
  bool bParried,
  bool bCriticalHit
) const
{
  if (Props.SourceCharacter != Props.TargetCharacter)
  {
    AAuraPlayerController* SourcePC = Cast<AAuraPlayerController>(Props.SourceController);

    if (SourcePC)
    {
      SourcePC->ShowDamageNumber(Damage, Props.TargetCharacter, bParried, bCriticalHit, false);
    }
    else if (AAuraPlayerController* TargetPC =
      Cast<AAuraPlayerController>(Props.TargetController)
    )
    {
      TargetPC->ShowDamageNumber(Damage, Props.TargetCharacter, bParried, bCriticalHit, true);
    }
  }
}

void UAuraAttributeSet::ShowHealFloatingText(const FEffectProperties& Props, float Heal) const
{
  AAuraPlayerController* SourcePC = Cast<AAuraPlayerController>(Props.SourceController);

  if (SourcePC)
  {
    SourcePC->ShowHealNumber(Heal, Props.TargetCharacter, false);
  }
  else if (AAuraPlayerController* TargetPC =
    Cast<AAuraPlayerController>(Props.TargetController)
  )
  {
    TargetPC->ShowHealNumber(Heal, Props.TargetCharacter, true);
  }
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
  if (Props.TargetCharacter->Implements<UCombatInterface>())
  {
    const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter);
    const ECharacterType TargetType = ICombatInterface::Execute_GetCharacterType(Props.TargetCharacter);
    
    // Stacks XP until the match is complete
    UAuraSystemsLibrary::StackXP(Props.SourceCharacter, TargetType, TargetLevel);
    
    /*
     * Old logic. Gives XP instantly on enemy death, only to the killer
     */
    // const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
    // FGameplayEventData Payload;
    // Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
    // Payload.EventMagnitude = XPReward;
    // UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
    // 	Props.SourceCharacter,
    // 	GameplayTags.Attributes_Meta_IncomingXP,
    // 	Payload
    // );
  }
}

ICombatInterface* UAuraAttributeSet::GetAvatarCombatInterface()
{
  if (AvatarCombatInterface == nullptr)
  {
    AvatarCombatInterface = Cast<ICombatInterface>(GetActorInfo()->AvatarActor);
  }

  return AvatarCombatInterface;
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Ferocity(const FGameplayAttributeData& OldFerocity) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Ferocity, OldFerocity);
}

void UAuraAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Agility, OldAgility);
}

void UAuraAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Toughness, OldToughness);
}

void UAuraAttributeSet::OnRep_Attunement(const FGameplayAttributeData& OldAttunement) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Attunement, OldAttunement);
}

void UAuraAttributeSet::OnRep_Willpower(const FGameplayAttributeData& OldWillpower) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Willpower, OldWillpower);
}

void UAuraAttributeSet::OnRep_Faith(const FGameplayAttributeData& OldFaith) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Faith, OldFaith);
}

void UAuraAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UAuraAttributeSet::OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CastSpeed, OldCastSpeed);
}

void UAuraAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UAuraAttributeSet::OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalRate, OldCriticalRate);
}

void UAuraAttributeSet
::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(
  const FGameplayAttributeData& OldManaRegeneration
)
const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet
::OnRep_ChaosResistance(const FGameplayAttributeData& OldChaosResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ChaosResistance, OldChaosResistance);
}

void UAuraAttributeSet
::OnRep_AirResistance(const FGameplayAttributeData& OldAirResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AirResistance, OldAirResistance);
}

void UAuraAttributeSet
::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet
::OnRep_WaterResistance(const FGameplayAttributeData& OldWaterResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, WaterResistance, OldWaterResistance);
}

void UAuraAttributeSet
::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_EarthResistance(
  const FGameplayAttributeData& OldEarthResistance
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, EarthResistance, OldEarthResistance);
}

void UAuraAttributeSet::OnRep_Dodge(const FGameplayAttributeData& OldDodge) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Dodge, OldDodge);
}

void UAuraAttributeSet::OnRep_Parry(const FGameplayAttributeData& OldParry) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Parry, OldParry);
}

void UAuraAttributeSet::OnRep_CooldownSpeed(
  const FGameplayAttributeData& OldCooldownSpeed
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CooldownSpeed, OldCooldownSpeed);
}

void UAuraAttributeSet::OnRep_Power(
  const FGameplayAttributeData& OldDamageMultiplier
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Power, OldDamageMultiplier);
}

void UAuraAttributeSet::OnRep_TimeDilation(const FGameplayAttributeData& OldTimeDilation) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, TimeDilation, OldTimeDilation);
}
