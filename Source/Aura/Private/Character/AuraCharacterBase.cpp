// Copyright Lucas Rossi


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/SummonAbility.h"
#include "Animation/CharacterAnimInstance.h"
#include "Components/BoxComponent.h"
#include "Components/TeamComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
  PrimaryActorTick.bCanEverTick = false;

  StatusEffectsManager = CreateDefaultSubobject<UStatusEffectsManager>("StatusEffectsManager");
  TopStatusEffectSceneComponent = CreateDefaultSubobject<USceneComponent>("TopStatusEffectSceneComponent");
  TopStatusEffectSceneComponent->SetupAttachment(GetRootComponent());
  TopStatusEffectSceneComponent->SetRelativeLocation(
    FVector(
      0.f,
      0.f,
      GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
    )
  );
  BottomStatusEffectSceneComponent = CreateDefaultSubobject<USceneComponent>("BottomStatusEffectSceneComponent");
  BottomStatusEffectSceneComponent->SetupAttachment(GetRootComponent());
  BottomStatusEffectSceneComponent->SetRelativeLocation(
    FVector(
      0.f,
      0.f,
      -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
    )
  );

  Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
  Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
  Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Weapon->bReceivesDecals = false;
  WeaponHitBox = CreateDefaultSubobject<UBoxComponent>("WeaponHitBox");
  WeaponHitBox->SetupAttachment(Weapon);
  WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  WeaponHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
  WeaponHitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

  TeamComponent = CreateDefaultSubobject<UTeamComponent>("TeamComponent");

  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ExcludeCharacters, ECR_Ignore);
  GetCapsuleComponent()->SetGenerateOverlapEvents(false);
  GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
  GetMesh()->SetCollisionResponseToChannel(ECC_ExcludeCharacters, ECR_Ignore);
  GetMesh()->SetGenerateOverlapEvents(true);
  GetMesh()->bReceivesDecals = false;

  CurrentWalkSpeed = DefaultWalkSpeed;
  GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
  GetCharacterMovement()->BrakingFriction = 1.f;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
  return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
  return HitReactMontage;
}

UAnimMontage* AAuraCharacterBase::GetStunMontage_Implementation()
{
  return StunMontage;
}

UAnimMontage* AAuraCharacterBase::GetDodgeMontage_Implementation()
{
  return DodgeMontage;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
  MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacterBase::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
  CombatTarget = InCombatTarget;
}

AActor* AAuraCharacterBase::GetCombatTarget_Implementation() const
{
  return CombatTarget;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
  return Weapon.Get();
}

USkeletalMeshComponent* AAuraCharacterBase::GetAvatarMesh_Implementation()
{
  return GetMesh();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
  UGameplayStatics::PlaySoundAtLocation(
    this,
    DeathSound,
    GetActorLocation(),
    GetActorRotation()
  );

  bDead = true;
  OnDeath.Broadcast(this);

  ReleaseWeapon();
  RagdollMesh(DeathImpulse);

  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  DissolveCharacter();
}

void AAuraCharacterBase::InitSummon(int32 TeamID)
{
  CharacterType = ECharacterType::Minion;
  TeamComponent->TeamID = TeamID;
  LifeSpanDuration = 2.f;
  SpawnDefaultController();

  OnSpawned();
}

void AAuraCharacterBase::BeginPlay()
{
  Super::BeginPlay();
}

void AAuraCharacterBase::ReleaseWeapon()
{
  Weapon->DetachFromComponent(
    FDetachmentTransformRules(
      EDetachmentRule::KeepWorld,
      true
    )
  );
  Weapon->SetSimulatePhysics(true);
  Weapon->SetEnableGravity(true);
  Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AAuraCharacterBase::RagdollMesh(const FVector& DeathImpulse)
{
  GetMesh()->SetSimulatePhysics(true);
  GetMesh()->SetEnableGravity(true);
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
  GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
  GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
  GetMesh()->bPauseAnims = true;
}

void AAuraCharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
  bHitReacting = NewCount > 0;

  GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : CurrentWalkSpeed;
}

void AAuraCharacterBase::HighlightActor_Implementation(AActor* InstigatorActor)
{
  int32 CustomDepth = 0;
  if (InstigatorActor == nullptr)
  {
    CustomDepth = CUSTOM_DEPTH_WHITE;
  }
  else if (UAuraAbilitySystemLibrary::AreActorsEnemies(this, InstigatorActor))
  {
    CustomDepth = CUSTOM_DEPTH_RED;
  }
  else if (UAuraAbilitySystemLibrary::AreActorsFriends(this, InstigatorActor))
  {
    CustomDepth = CUSTOM_DEPTH_GREEN;
  }

  GetMesh()->SetRenderCustomDepth(true);
  GetMesh()->SetCustomDepthStencilValue(CustomDepth);
  Weapon->SetRenderCustomDepth(true);
  Weapon->SetCustomDepthStencilValue(CustomDepth);
}

void AAuraCharacterBase::UnHighlightActor_Implementation()
{
  GetMesh()->SetRenderCustomDepth(false);
  Weapon->SetRenderCustomDepth(false);
}

FVector AAuraCharacterBase::GetAbilitySocketLocation_Implementation(
  const FName SocketName,
  bool bSocketInWeapon
)
{
  if (bSocketInWeapon && IsValid(Weapon))
  {
    return Weapon->GetSocketLocation(SocketName);
  }
  return GetMesh()->GetSocketLocation(SocketName);
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
  return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
  return this;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
  return BloodEffect;
}

USoundBase* AAuraCharacterBase::GetHitSound_Implementation()
{
  return HitSound;
}

ECharacterType AAuraCharacterBase::GetCharacterType_Implementation()
{
  return CharacterType;
}

FOnASCRegistered& AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
  return OnASCRegistered;
}

FOnDeath& AAuraCharacterBase::GetOnDeathDelegate()
{
  return OnDeath;
}

void AAuraCharacterBase::ApplyKnockback_Implementation(const FVector& KnockbackForce)
{
  if (GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Immunities_Force)) return;

  GetCharacterMovement()->StopMovementImmediately();
  GetCharacterMovement()->bUseSeparateBrakingFriction = true;
  LaunchCharacter(KnockbackForce, true, true);

  GetWorld()->GetTimerManager().SetTimer(
    VelocityCheckTimer,
    this,
    &AAuraCharacterBase::CheckVelocityNearStop,
    0.2f,
    true,
    0.2f
  );
}

void AAuraCharacterBase::ApplyForce_Implementation(const FVector& Force)
{
  if (GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Immunities_Force)) return;

  GetCharacterMovement()->AddForce(Force);
}

void AAuraCharacterBase::ApplyAttraction_Implementation(
  const FVector& AttractionPoint,
  float DeltaTime,
  float InterpSpeed
)
{
  if (GetAuraASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Immunities_Force)) return;

  SetActorLocation(
    FMath::VInterpConstantTo(
      GetActorLocation(),
      AttractionPoint,
      DeltaTime,
      InterpSpeed
    )
  );
}

USceneComponent* AAuraCharacterBase::GetTopStatusEffectSceneComponent_Implementation()
{
  return TopStatusEffectSceneComponent;
}

USceneComponent* AAuraCharacterBase::GetBottomStatusEffectSceneComponent_Implementation()
{
  return BottomStatusEffectSceneComponent;
}

UBoxComponent* AAuraCharacterBase::EnableWeaponCollision_Implementation(bool bEnable)
{
  if (bEnable)
  {
    WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  }
  else
  {
    WeaponHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }

  return WeaponHitBox;
}

bool AAuraCharacterBase::IsFriend_Implementation(AActor* Actor)
{
  return UAuraAbilitySystemLibrary::AreActorsFriends(this, Actor);
}

bool AAuraCharacterBase::IsEnemy_Implementation(AActor* Actor)
{
  return UAuraAbilitySystemLibrary::AreActorsEnemies(this, Actor);
}

void AAuraCharacterBase::SetCustomDepth_Implementation(int32 Value)
{
  GetMesh()->SetRenderCustomDepth(Value > 0);
  GetMesh()->SetCustomDepthStencilValue(Value);
}

float AAuraCharacterBase::GetCapsuleHalfHeight_Implementation() const
{
  return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(
  TSubclassOf<UGameplayEffect> GameplayEffectClass,
  float Level
) const
{
  UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
  check(IsValid(ASC));
  check(GameplayEffectClass);

  FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
  ContextHandle.AddSourceObject(this);
  const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
    GameplayEffectClass,
    Level,
    ContextHandle
  );
  ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}

void AAuraCharacterBase::InitializeAttributes()
{
  if (DefaultSecondaryAttributes) ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
  if (DefaultVitalAttributes) ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
  if (DefaultRegenerationEffect) ApplyEffectToSelf(DefaultRegenerationEffect, 1.f);

  if (!NativeEffects.IsEmpty())
  {
    for (const auto Effect : NativeEffects)
    {
      ApplyEffectToSelf(Effect, 1.f);
    }
  }
}

void AAuraCharacterBase::InitializeAbilities()
{
  if (!HasAuthority()) return;

  const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
  AbilitySystemComponent
    ->RegisterGameplayTagEvent(
      GameplayTags.StatusEffects_Incapacitation_HitReact,
      EGameplayTagEventType::NewOrRemoved
    )
    .AddUObject(
      this,
      &AAuraCharacterBase::HitReactTagChanged
    );

  GetAuraASC()->AddCharacterAbilities(NativeBaseAbilities, GameplayTags.Abilities_Status_Native);
  GetAuraASC()->AddCharacterAbilities(NativeCharacterAbilities, GameplayTags.Abilities_Status_Native);
}

void AAuraCharacterBase::DissolveCharacter()
{
  if (IsValid(DissolveMaterialInstance))
  {
    UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic
      ::Create(DissolveMaterialInstance, this);
    GetMesh()->SetMaterial(0, DynamicMatInst);

    StartDissolveTimeline(DynamicMatInst);
  }

  if (IsValid(WeaponDissolveMaterialInstance))
  {
    UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic
      ::Create(WeaponDissolveMaterialInstance, this);
    Weapon->SetMaterial(0, DynamicMatInst);

    StartWeaponDissolveTimeline(DynamicMatInst);
  }
}

void AAuraCharacterBase::CheckVelocityNearStop()
{
  if (GetVelocity().Length() > 1.f) return;

  const FGameplayEventData EventData = FGameplayEventData();
  GetAuraASC()->HandleGameplayEvent(
    FAuraGameplayTags::Get().StatusEffects_Incapacitation_Knockback,
    &EventData
  );

  GetCharacterMovement()->bUseSeparateBrakingFriction = false;

  GetWorld()->GetTimerManager().ClearTimer(VelocityCheckTimer);
}

void AAuraCharacterBase::ChangeActionSpeed(float InActionSpeed)
{
  GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * InActionSpeed;

  UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
  if (AnimInstance)
  {
    AnimInstance->SetActionPlayRate(InActionSpeed);
  }
}

UAuraAbilitySystemComponent* AAuraCharacterBase::GetAuraASC()
{
  if (AuraASC == nullptr)
  {
    AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
  }

  return AuraASC;
}
