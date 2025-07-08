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
	
	/*
	 * Primary attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ferocity, Category = "Primary Attributes")
	FGameplayAttributeData Ferocity;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Ferocity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Category = "Primary Attributes")
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Toughness);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attunement, Category = "Primary Attributes")
	FGameplayAttributeData Attunement;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Attunement);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Willpower, Category = "Primary Attributes")
	FGameplayAttributeData Willpower;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Willpower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Faith, Category = "Primary Attributes")
	FGameplayAttributeData Faith;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Faith);

	/*
	 * Secondary attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Secondary Attributes")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, AttackSpeed);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CastSpeed, Category = "Secondary Attributes")
  FGameplayAttributeData CastSpeed;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CastSpeed);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "Secondary Attributes")
  FGameplayAttributeData MovementSpeed;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownSpeed, Category = "Secondary Attributes")
  FGameplayAttributeData CooldownSpeed;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CooldownSpeed);

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

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dodge, Category = "Secondary Attributes")
  FGameplayAttributeData Dodge;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Dodge);

  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Parry, Category = "Secondary Attributes")
  FGameplayAttributeData Parry;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Parry);
	
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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ChaosResistance, Category = "Resistance Attributes")
	FGameplayAttributeData ChaosResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ChaosResistance);
  

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaterResistance, Category = "Resistance Attributes")
	FGameplayAttributeData WaterResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, WaterResistance);
  
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AirResistance, Category = "Resistance Attributes")
	FGameplayAttributeData AirResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, AirResistance);
  
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EarthResistance, Category = 
	"Resistance Attributes")
	FGameplayAttributeData EarthResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, EarthResistance);
  
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = 
	"Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);

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
	void OnRep_Ferocity(const FGameplayAttributeData& OldFerocity) const;

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;
	
	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldToughness) const;
	
	UFUNCTION()
	void OnRep_Attunement(const FGameplayAttributeData& OldAttunement) const;
	
	UFUNCTION()
	void OnRep_Willpower(const FGameplayAttributeData& OldWillpower) const;

	UFUNCTION()
	void OnRep_Faith(const FGameplayAttributeData& OldFaith) const;

	/*
	 * Secondary
	 */

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;

  UFUNCTION()
  void OnRep_CastSpeed(const FGameplayAttributeData& OldCastSpeed) const;

  UFUNCTION()
  void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

  UFUNCTION()
  void OnRep_CooldownSpeed(const FGameplayAttributeData& OldCooldownSpeed) const;

	UFUNCTION()
	void OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalRate(const FGameplayAttributeData& OldCriticalRate) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
  
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

  UFUNCTION()
  void OnRep_Dodge(const FGameplayAttributeData& OldDodge) const;
  
  UFUNCTION()
  void OnRep_Parry(const FGameplayAttributeData& OldParry) const;

	/*
	 * Resistance
	 */

	UFUNCTION()
	void OnRep_AirResistance(const FGameplayAttributeData& OldAirResistance) const;

	UFUNCTION()
	void OnRep_ChaosResistance(const FGameplayAttributeData& OldChaosResistance) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_WaterResistance(const FGameplayAttributeData& OldWaterResistance) const;

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;

	UFUNCTION()
	void OnRep_EarthResistance(const FGameplayAttributeData& OldEarthResistance) const;

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
};
