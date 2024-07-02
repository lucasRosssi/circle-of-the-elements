// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Components/StatusEffectsManager.h"
#include "Enums/CharacterType.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "Enums/CharacterName.h"
#include "Game/TeamComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UImage;
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
	ECharacterName GetCharacterName() const { return CharacterName; }
	
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	/** Combat Interface */
	virtual UAnimMontage* GetDodgeMontage_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetStunMontage_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual USkeletalMeshComponent* GetAvatarMesh_Implementation() override;
	virtual FVector GetAbilitySocketLocation_Implementation(const FName SocketName, bool bSocketInWeapon = true) override;;
	virtual void Die(const FVector& DeathImpulse = FVector::ZeroVector) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual USoundBase* GetHitSound_Implementation() override;
	virtual ECharacterType GetCharacterType_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual void ApplyForce_Implementation(const FVector& InForce) override;
	virtual USceneComponent* GetTopStatusEffectSceneComponent_Implementation() override;
	virtual USceneComponent* GetBottomStatusEffectSceneComponent_Implementation() override;
	virtual UCapsuleComponent* EnableWeaponCollision_Implementation(bool bEnable) override;
	/** end Combat Interface */

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float LifeSpanDuration = 10.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CharacterAbilities;

	void InitSummon(int32 TeamID);

	void ChangeMovementSpeed(float InMovementSpeed);
	void ChangeActionSpeed(float InActionSpeed);

	int32 GetTeamID() { return TeamComponent->TeamID; }
	void SetTeamID(int32 InID) { TeamComponent->TeamID = InID; }

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSummoned();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Montages|Combat")
	TObjectPtr<UAnimMontage> DodgeMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages|Combat")
	TObjectPtr<UAnimMontage> StunMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCapsuleComponent> WeaponCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UTeamComponent> TeamComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
	ECharacterName CharacterName = ECharacterName::ECN_Undefined;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
	ECharacterType CharacterType = ECharacterType::ECT_Regular;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultRegenerationEffect;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character Defaults", meta=( DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TObjectPtr<UObject> Portrait;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character Defaults")
	FColor MainColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Speed")
	float DefaultWalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Speed")
	float CurrentWalkSpeed = DefaultWalkSpeed;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusEffectsManager> StatusEffectsManager;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> TopStatusEffectSceneComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BottomStatusEffectSceneComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> BaseAbilities;
};
