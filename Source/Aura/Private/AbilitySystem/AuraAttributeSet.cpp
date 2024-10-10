// Copyright Lucas Rossi


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/AttributeSetInterface.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/DamageCameraShake.h"

UAuraAttributeSet::UAuraAttributeSet()
{
  InitHealth(100.f);
  InitMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(
  TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  // Primary
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Prime, COND_None, REPNOTIFY_Always);

  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Constitution, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Wisdom, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Charisma, COND_None, REPNOTIFY_Always);

  // Secondary
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ActionSpeed, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalRate, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ParryChance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Restoration, COND_None, REPNOTIFY_Always);

  // Vital
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

  // Resistance
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, EnergyResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, NecroticResistance, COND_None, REPNOTIFY_Always);

  // Special
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Power, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, TimeDilation, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, SoulEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, IceEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, NecroticEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneEssenceMultiplier, COND_None, REPNOTIFY_Always);
  DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalEssenceMultiplier, COND_None, REPNOTIFY_Always);
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

  if (Attribute == PrimeAttribute)
  {
    SetPrime(NewValue);
  }

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

  if (Attribute == GetActionSpeedAttribute())
  {
    IAttributeSetInterface::Execute_SetActionSpeed(GetOwningActor(), NewValue);
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
      ASC->HandleGameplayEvent(AuraTags.Abilities_Passive_Common_SecondWind, &EventData);
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
    if (Props.TargetCharacter->IsPlayerControlled() && Props.TargetCharacter->IsLocallyControlled())
    {
      UGameplayStatics::GetPlayerCameraManager(
        Props.TargetAvatarActor,
        0)->StartCameraShake(UDamageCameraShake::StaticClass());
    }
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
  if (LocalIncomingForce > 0.f)
  {
    SetIncomingForce(0.f);

    const FVector ForwardVector =
      UAuraAbilitySystemLibrary::GetForwardVector(Props.EffectContextHandle);
    const FVector KnockbackForce = ForwardVector * LocalIncomingForce;

    Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
    Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
  }
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

void UAuraAttributeSet::AssignPrimeAttribute(const FGameplayTag& InAttributeTag)
{
  const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

  if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Strength))
  {
    PrimeAttribute = GetStrengthAttribute();
  }
  else if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Dexterity))
  {
    PrimeAttribute = GetDexterityAttribute();
  }
  else if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Constitution))
  {
    PrimeAttribute = GetConstitutionAttribute();
  }
  else if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Intelligence))
  {
    PrimeAttribute = GetIntelligenceAttribute();
  }
  else if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Wisdom))
  {
    PrimeAttribute = GetWisdomAttribute();
  }
  else if (InAttributeTag.MatchesTagExact(GameplayTags.Attributes_Primary_Charisma))
  {
    PrimeAttribute = GetCharismaAttribute();
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
    const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForTypeAndLevel(
      Props.TargetCharacter,
      TargetType,
      TargetLevel
    );

    // Stacks XP until the room is cleared
    UAuraAbilitySystemLibrary::StackEncounterXP(Props.TargetCharacter, XPReward);

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

void UAuraAttributeSet::OnRep_Prime(const FGameplayAttributeData& OldPrime) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Prime, OldPrime);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Dexterity, OldDexterity);
}

void UAuraAttributeSet::OnRep_Constitution(const FGameplayAttributeData& OldConstitution) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Constitution, OldConstitution);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Wisdom, OldWisdom);
}

void UAuraAttributeSet::OnRep_Charisma(const FGameplayAttributeData& OldCharisma) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Charisma, OldCharisma);
}

void UAuraAttributeSet::OnRep_ActionSpeed(const FGameplayAttributeData& OldActionSpeed) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ActionSpeed, OldActionSpeed);
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
::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UAuraAttributeSet
::OnRep_EnergyResistance(const FGameplayAttributeData& OldEnergyResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, EnergyResistance, OldEnergyResistance);
}

void UAuraAttributeSet
::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet
::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, IceResistance, OldIceResistance);
}

void UAuraAttributeSet
::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_NecroticResistance(
  const FGameplayAttributeData& OldNecroticResistance
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, NecroticResistance, OldNecroticResistance);
}

void UAuraAttributeSet::OnRep_ParryChance(const FGameplayAttributeData& OldParryChance) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ParryChance, OldParryChance);
}

void UAuraAttributeSet::OnRep_Restoration(const FGameplayAttributeData& OldRestoration) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Restoration, OldRestoration);
}

void UAuraAttributeSet::OnRep_CooldownReduction(
  const FGameplayAttributeData& OldCooldownReduction
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CooldownReduction, OldCooldownReduction);
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

void UAuraAttributeSet::OnRep_SoulEssenceMultiplier(const FGameplayAttributeData& OldSoulEssenceMultiplier) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, SoulEssenceMultiplier, OldSoulEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_FireEssenceMultiplier(const FGameplayAttributeData& OldFireEssenceMultiplier) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireEssenceMultiplier, OldFireEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_IceEssenceMultiplier(const FGameplayAttributeData& OldIceEssenceMultiplier) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, IceEssenceMultiplier, OldIceEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_LightningEssenceMultiplier(
  const FGameplayAttributeData& OldLightningEssenceMultiplier
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningEssenceMultiplier, OldLightningEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_NecroticEssenceMultiplier(
  const FGameplayAttributeData& OldNecroticEssenceMultiplier
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, NecroticEssenceMultiplier, OldNecroticEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_ArcaneEssenceMultiplier(const FGameplayAttributeData& OldArcaneEssenceMultiplier) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneEssenceMultiplier, OldArcaneEssenceMultiplier);
}

void UAuraAttributeSet::OnRep_PhysicalEssenceMultiplier(
  const FGameplayAttributeData& OldPhysicalEssenceMultiplier
) const
{
  GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalEssenceMultiplier, OldPhysicalEssenceMultiplier);
}
