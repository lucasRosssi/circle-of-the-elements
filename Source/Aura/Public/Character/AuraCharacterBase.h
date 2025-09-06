// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Components/StatusEffectsManager.h"
#include "Enums/CharacterType.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "Enums/CharacterName.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/TargetInterface.h"
#include "AuraCharacterBase.generated.h"

class UCharacterAnimInstance;
class UAuraAbilitySystemComponent;
class UBoxComponent;
class UImage;
class USummonAbility;
class UNiagaraSystem;
class UTeamComponent;
struct FGameplayTag;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAuraAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElementalFlowChangedDelegate, const FGameplayTag&, ElementalFlowTag);

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public
                                    ICombatInterface, public ITargetInterface
{
  GENERATED_BODY()

public:
  AAuraCharacterBase();
  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
  UAuraAttributeSet* GetAttributeSet() const { return AttributeSet; }
  ECharacterName GetCharacterName() const { return CharacterName; }

  virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

  UFUNCTION(NetMulticast, Reliable)
  virtual void MulticastHandleDeath(const FVector& DeathImpulse);

  /** Target Interface */
  virtual void HighlightActor_Implementation(AActor* InstigatorActor = nullptr) override;
  virtual void UnHighlightActor_Implementation() override;
  /** end Target Interface */

  /** Combat Interface */
  virtual UAnimMontage* GetDodgeMontage_Implementation() override;
  virtual UAnimMontage* GetHitReactMontage_Implementation() override;
  virtual UAnimMontage* GetStunMontage_Implementation() override;
  virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
  virtual AActor* GetCombatTarget_Implementation() const override;
  virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
  virtual USkeletalMeshComponent* GetAvatarMesh_Implementation() override;
  virtual FVector GetAbilitySocketLocation_Implementation(const FName SocketName, bool bSocketInWeapon = true) override;
  virtual void Die(const FVector& DeathImpulse = FVector::ZeroVector) override;
  virtual void CallDie_Implementation(const FVector& DeathImpulse) override;
  virtual bool IsDead_Implementation() const override;
  virtual AActor* GetAvatar_Implementation() override;
  virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
  virtual USoundBase* GetHitSound_Implementation() override;
  virtual UNiagaraComponent* GetBodyNiagaraComponent_Implementation() override;
  virtual void ActivateBodyNiagara_Implementation(UNiagaraSystem* NiagaraSystem) override;
  virtual void DeactivateBodyNiagara_Implementation() override;
  virtual ECharacterType GetCharacterType_Implementation() override;
  virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
  virtual FOnDeath& GetOnDeathDelegate() override;
  virtual void ApplyKnockback_Implementation(const FVector& KnockbackForce) override;
  virtual void ApplyForce_Implementation(const FVector& Force) override;
  virtual void ApplyAttraction_Implementation(const FVector& AttractionPoint, float DeltaTime, float InterpSpeed) override;
  virtual UBoxComponent* EnableWeaponCollision_Implementation(bool bEnable) override;
  virtual bool IsFriend_Implementation(AActor* Actor) override;
  virtual bool IsEnemy_Implementation(AActor* Actor) override;
  virtual void SetCustomDepth_Implementation(int32 Value) override;
  virtual void SetStateTag_Implementation(const FGameplayTag& StateTag) override;
  /** END Combat Interface */

  void InitSummon(int32 TeamID);
  UFUNCTION(BlueprintImplementableEvent)
  void OnSpawned();

  void ChangeMovementSpeed(float InMovementSpeed);

  void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
  
  UTeamComponent* GetTeamComponent() const { return TeamComponent; }

  UAuraAbilitySystemComponent* GetAuraASC();

  FOnASCRegistered OnASCRegistered;
  UPROPERTY(BlueprintAssignable)
  FOnDeath OnDeath;

  UPROPERTY(BlueprintReadOnly, Category="Combat")
  bool bHitReacting = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Defaults|Combat")
  float LifeSpanDuration = 10.f;

  UPROPERTY(BlueprintReadWrite, Category="Combat")
  TObjectPtr<AActor> CombatTarget;

  UPROPERTY(EditDefaultsOnly, Category = "Character Defaults|Abilities|Startup", meta=(DisplayPriority=0))
  TArray<TSubclassOf<UGameplayAbility>> NativeBaseAbilities;
  UPROPERTY(EditDefaultsOnly, Category="Character Defaults|Abilities|Startup", meta=(DisplayPriority=1))
  TArray<TSubclassOf<UGameplayAbility>> NativeCharacterAbilities;
  UPROPERTY(EditDefaultsOnly, Category="Character Defaults|Effects|Startup", meta=(DisplayPriority=3))
  TArray<TSubclassOf<UGameplayEffect>> NativeEffects;

protected:
  virtual void BeginPlay() override;

  virtual void InitAbilityActorInfo();
  virtual void InitializeAbilities();
  virtual void InitializeAttributes();
  
  void RegisterElementalFlowEvents();
  
  void ReleaseWeapon();
  void RagdollMesh(const FVector& DeathImpulse = FVector::ZeroVector);

  void DissolveCharacter();
  UFUNCTION(BlueprintImplementableEvent)
  void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
  UFUNCTION(BlueprintImplementableEvent)
  void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
  UFUNCTION()
  void OnElementalFlowChange(const FGameplayTag ElementalFlowTag, int32 NewCount);

  UPROPERTY(BlueprintAssignable)
  FOnElementalFlowChangedDelegate ElementalFlowChangedDelegate;

  UPROPERTY()
  TObjectPtr<UAuraAttributeSet> AttributeSet;
  UPROPERTY()
  TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
  TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
  TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
  TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
  TSubclassOf<UGameplayEffect> DefaultRegenerationEffect;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Defaults|Montages|Combat")
  TObjectPtr<UAnimMontage> DodgeMontage;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults|Montages|Combat")
  TObjectPtr<UAnimMontage> HitReactMontage;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults|Montages|Combat")
  TObjectPtr<UAnimMontage> StunMontage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults|Combat")
  TObjectPtr<UNiagaraSystem> BloodEffect;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults|Combat")
  TObjectPtr<USoundBase> HitSound;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults|Combat")
  TObjectPtr<USoundBase> DeathSound;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character Defaults|Combat")
  float DeathImpulseRatio = 1.f;


  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  TObjectPtr<USkeletalMeshComponent> Weapon;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  TObjectPtr<UBoxComponent> WeaponHitBox;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
  FName WeaponSocketName;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  TObjectPtr<UTeamComponent> TeamComponent;

  bool bDead = false;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
  ECharacterName CharacterName = ECharacterName::Undefined;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
  ECharacterType CharacterType = ECharacterType::Regular;
  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category= "Character Defaults",
    meta=( DisplayThumbnail="true", AllowedClasses=
      "/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
      DisallowedClasses = "/Script/MediaAssets.MediaTexture")
  )
  TObjectPtr<UObject> Portrait;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character Defaults")
  FColor MainColor;

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Character Defaults|Speed")
  float DefaultWalkSpeed = 600.f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Character Defaults|Speed")
  float CurrentWalkSpeed = DefaultWalkSpeed;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UStatusEffectsManager> StatusEffectsManager;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UNiagaraComponent> BodyNiagaraComponent;

private:
  UCharacterAnimInstance* GetAnimInstance();

  TWeakObjectPtr<UCharacterAnimInstance> AnimInstance = nullptr;
  
  void EndKnockback();

  FTimerHandle KnockbackEndTimer;

  UPROPERTY()
  UAuraAbilitySystemComponent* AuraASC = nullptr;
};
