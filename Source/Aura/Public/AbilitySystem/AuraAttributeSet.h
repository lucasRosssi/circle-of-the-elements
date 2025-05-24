// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ICombatInterface;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}
	
	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	void HandleDeath(const FEffectProperties& Props, float DeathImpulseMagnitude);
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	void AssignPrimeAttribute(const FGameplayTag& InAttributeTag);

	/*
	 * Primary attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Prime;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Prime);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Dexterity);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Constitution, Category = "Primary Attributes")
	FGameplayAttributeData Constitution;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Constitution);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Wisdom, Category = "Primary Attributes")
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Wisdom);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charisma, Category = "Primary Attributes")
	FGameplayAttributeData Charisma;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Charisma);

	/*
	 * Secondary attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ActionSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData ActionSpeed;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ActionSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownReduction, Category = "Secondary Attributes")
  FGameplayAttributeData CooldownReduction;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CooldownReduction);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalRate, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalRate;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ParryChance, Category = "Secondary Attributes")
  FGameplayAttributeData ParryChance;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ParryChance);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Restoration, Category = "Secondary Attributes")
  FGameplayAttributeData Restoration;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Restoration);
	
	/*
	 * Vital attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	/*
	 * Resistance attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyResistance, Category = "Resistance Attributes")
	FGameplayAttributeData EnergyResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, EnergyResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IceResistance, Category = "Resistance Attributes")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IceResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = 
	"Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_NecroticResistance, Category = 
	"Resistance Attributes")
	FGameplayAttributeData NecroticResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, NecroticResistance);

	/*
	 * Special attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Power, Category = "Special Attributes")
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Power);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TimeDilation, Category = "Special Attributes")
	FGameplayAttributeData TimeDilation;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, TimeDilation);

  // Resources



	/*
	 * Meta attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDoT;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDoT);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingForce;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingForce);
  
  UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
  FGameplayAttributeData IncomingHeal;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingHeal);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);
	
	/*
	 * Vital
	 */

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	/*
	 * Primary
	 */
	UFUNCTION()
	void OnRep_Prime(const FGameplayAttributeData& OldPrime) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Constitution(const FGameplayAttributeData& OldConstitution) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const;

	UFUNCTION()
	void OnRep_Charisma(const FGameplayAttributeData& OldCharisma) const;

	/*
	 * Secondary
	 */

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_ActionSpeed(const FGameplayAttributeData& OldActionSpeed) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

  UFUNCTION()
  void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const;

	UFUNCTION()
	void OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
  
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

  UFUNCTION()
  void OnRep_ParryChance(const FGameplayAttributeData& OldParryChance) const;
  
  UFUNCTION()
  void OnRep_Restoration(const FGameplayAttributeData& OldRestoration) const;

	/*
	 * Resistance
	 */

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	UFUNCTION()
	void OnRep_EnergyResistance(const FGameplayAttributeData& OldEnergyResistance) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_NecroticResistance(const FGameplayAttributeData& OldNecroticResistance) const;

	/*
	 * Special
	 */

	UFUNCTION()
	void OnRep_Power(const FGameplayAttributeData& OldDamageMultiplier) const;

	UFUNCTION()
	void OnRep_TimeDilation(const FGameplayAttributeData& OldTimeDilation) const;

private:
	void HandleIncomingDamage(const FEffectProperties& Props, const FGameplayAttribute& Attribute);
	void HandleIncomingXP(const FEffectProperties& Props);
	void HandleKnockback(const FEffectProperties& Props);
	void HandleHeal(const FEffectProperties& Props);
  void SetEffectProperties(
		const FGameplayEffectModCallbackData& Data,
		FEffectProperties& Props
		) const;
	void ShowDamageFloatingText(
		const FEffectProperties& Props,
		float Damage,
		bool bParried,
		bool bCriticalHit
		)	const;
  void ShowHealFloatingText(
    const FEffectProperties& Props,
    float Heal
    )	const;
	void SendXPEvent(const FEffectProperties& Props);

	ICombatInterface* GetAvatarCombatInterface();
	ICombatInterface* AvatarCombatInterface;

	bool bAvatarDead = false;

	FGameplayAttribute PrimeAttribute = FGameplayAttribute();
};
