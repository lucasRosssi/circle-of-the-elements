// Copyright Lucas Rossi


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/AttributeSetInterface.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/MainPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(100.f);
	InitMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Finesse, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ActionSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, StaminaEfficiency, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	// Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, EnergyResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, NecroticResistance, COND_None, REPNOTIFY_Always);
	
	// Special
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ParryChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, DamageMultiplier, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue
)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& 
                                               NewValue) const
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

	if (Attribute == GetActionSpeedAttribute())
	{
		IAttributeSetInterface::Execute_SetActionSpeed(GetOwningActor(), NewValue);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& Props) const
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
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent
		(Props.TargetAvatarActor);
	}
}


void UAuraAttributeSet::HandleIncomingDamage(
	const FEffectProperties& Props,
	const FGameplayAttribute& Attribute
	)
{
	if (GetIncomingDamage() + GetIncomingDoT() <= 0.f) return;
	
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
		const float SourceDamageMultiplier = Props.SourceASC
			->GetNumericAttribute(GetDamageMultiplierAttribute());
		LocalIncomingDamage = GetIncomingDamage() * SourceDamageMultiplier;
		SetIncomingDamage(0.f);
		bParried = UAuraAbilitySystemLibrary::IsParried(Props.EffectContextHandle);
		bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
	}
	
	const float NewHealth = GetHealth() - LocalIncomingDamage;
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

	const bool bFatal = NewHealth <= 0.f;
	if (bFatal)
	{
		const float DeathImpulseMagnitude = bDoT ? 0.f : FMath::Min(250 * LocalIncomingDamage, 20000.f);
		HandleDeath(Props, DeathImpulseMagnitude);
	}
	else if (!bDoT && UAuraAbilitySystemLibrary::GetApplyHitReact(Props.EffectContextHandle))
	{
		const FGameplayTagContainer TagContainer({
			FAuraGameplayTags::Get().Abilities_Reaction_HitReact
		 });
		Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
	}
	
	ShowFloatingText(Props, LocalIncomingDamage, bParried, bCriticalHit);
	
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
}

void UAuraAttributeSet::ShowFloatingText(
	const FEffectProperties& Props,
	float Damage,
	bool bParried,
	bool bCriticalHit
) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		AMainPlayerController* SourcePC = Cast<AMainPlayerController>(Props.SourceController);

		if (SourcePC)
		{
			SourcePC->ShowDamageNumber(Damage, Props.TargetCharacter, bParried, bCriticalHit, false);
		} else if (AMainPlayerController* TargetPC =
			Cast<AMainPlayerController>(Props.TargetController)
			)
		{
			TargetPC->ShowDamageNumber(Damage, Props.TargetCharacter, bParried, bCriticalHit, true);
		}
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

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Props.SourceCharacter,
			GameplayTags.Attributes_Meta_IncomingXP,
			Payload
		);
	}
}

ICombatInterface* UAuraAttributeSet::GetAvatarCombatInterface()
{
	if (AvatarCombatInterface == nullptr)
	{
		AvatarCombatInterface =	Cast<ICombatInterface>(GetActorInfo()->AvatarActor);
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

void UAuraAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Stamina, OldStamina);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Toughness, OldToughness);
}

void UAuraAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Agility, OldAgility);
}

void UAuraAttributeSet::OnRep_Finesse(const FGameplayAttributeData& OldFinesse) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Finesse, OldFinesse);
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

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) 
const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxStamina, OldMaxStamina);
}

void UAuraAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UAuraAttributeSet
::OnRep_StaminaEfficiency(const FGameplayAttributeData& OldStaminaEfficiency) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, StaminaEfficiency, OldStaminaEfficiency);
}

void UAuraAttributeSet
::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
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
	const FGameplayAttributeData& OldNecroticResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, NecroticResistance, OldNecroticResistance);
}

void UAuraAttributeSet::OnRep_ParryChance(const FGameplayAttributeData& OldParryChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ParryChance, OldParryChance);
}

void UAuraAttributeSet::OnRep_CooldownReduction(
	const FGameplayAttributeData& OldCooldownReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CooldownReduction, OldCooldownReduction);
}

void UAuraAttributeSet::OnRep_DamageMultiplier(
	const FGameplayAttributeData& OldDamageMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, DamageMultiplier, OldDamageMultiplier);
}
