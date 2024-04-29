// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class USummonAbility;
class UNiagaraSystem;
class UTeamComponent;
struct FGameplayTag;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public 
ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetDodgeMontage_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual FVector GetAbilitySocketLocation_Implementation(const FName SocketName, bool bSocketInWeapon = true) override;;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual USoundBase* GetHitSound_Implementation() override;
	/** end Combat Interface */
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float LifeSpanDuration = 10.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsMinion = false;

	void OnSummon();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSummoned();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Montages|Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UTeamComponent> TeamComponent;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	/* Dissolve Effects */
	void DissolveCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;

private:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	float DefaultWalkSpeed;
};
