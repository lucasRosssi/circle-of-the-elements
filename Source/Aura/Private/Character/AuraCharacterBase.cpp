// Copyright Lucas Rossi


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(
		EDetachmentRule::KeepWorld,
		true
	));
	MulticastHandleDeath();
}

void AAuraCharacterBase::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraCharacterBase::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	bDead = true;
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DissolveCharacter();
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : DefaultWalkSpeed;
}

FVector AAuraCharacterBase::GetWeaponSocketLocation_Implementation()
{
	return Weapon->GetSocketLocation(WeaponSocketName);
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
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
	ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	AbilitySystemComponent
		->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved
		)
		.AddUObject(
			this,
			&AAuraCharacterBase::HitReactTagChanged
		);
	
	UAuraAbilitySystemComponent* AuraASC =
		CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->AddCharacterAbilities(StartupAbilities);
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
