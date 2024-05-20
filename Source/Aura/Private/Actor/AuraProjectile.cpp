// Copyright Lucas Rossi


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/ProjectileEffect.h"
#include "Aura/Aura.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	if (ProjectileNiagaraEffect)
	{
		ProjectileNiagaraEffectActor = GetWorld()->SpawnActor<AProjectileEffect>(
			ProjectileNiagaraEffect,
			GetActorLocation(),
			GetActorRotation()
		);

		const FAttachmentTransformRules Rules = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			false
		);
		ProjectileNiagaraEffectActor->AttachToActor(this, Rules);
	}
	
	if (MuzzleEffect)
	{
		UNiagaraComponent* MuzzleNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			MuzzleEffect,
			GetActorLocation(),
			GetActorRotation()
		);
		if (MuzzleNiagara)
		{
			MuzzleNiagara->SetNiagaraVariableFloat(FString("Scale"), MuzzleEffectScale);
		}
	}
		
	if (MuzzleSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			MuzzleSound,
			GetActorLocation(),
			GetActorRotation(),
			MuzzleSoundVolume,
			MuzzleSoundPitch
		);
	}
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ImpactSound,
			GetActorLocation(),
			FRotator::ZeroRotator,
			ImpactSoundVolume,
			ImpactSoundPitch
		);
		UNiagaraComponent* ImpactNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ImpactEffect,
			GetActorLocation(),
			GetActorRotation()
		);
		if (ImpactNiagara)
		{
			ImpactNiagara->SetNiagaraVariableFloat(FString("Scale"), ImpactEffectScale);
		}

		if (ProjectileNiagaraEffectActor)
		{
			ProjectileNiagaraEffectActor->DeactivateNiagara();
			ProjectileNiagaraEffectActor = nullptr;
		}
	}
	
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	const TSharedPtr<FGameplayEffectSpec> SpecHandleData = DamageEffectSpecHandle.Data;
	if (!SpecHandleData.IsValid()) return;
	
	const AActor* EffectCauser = SpecHandleData.Get()->GetContext().GetEffectCauser();
	
	if (
		// OtherActor is the instigator
		EffectCauser == OtherActor ||
		// OtherActor is friend
		UAuraAbilitySystemLibrary::AreActorsFriends(EffectCauser, OtherActor)
	)
	{
		return;
	}

	if (UAuraAbilitySystemLibrary::IsTargetInvulnerable(OtherActor))
	{
		return;
	}
	
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation(),
		FRotator::ZeroRotator
	);
	UNiagaraComponent* ImpactNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ImpactEffect,
		GetActorLocation(),
		GetActorRotation()
	);
	if (ImpactNiagara)
	{
		ImpactNiagara->SetNiagaraVariableFloat(FString("Scale"), ImpactEffectScale);
	}
	ImpactEffect->GetExposedParameters();

	if (ProjectileNiagaraEffectActor)
	{
		ProjectileNiagaraEffectActor->DeactivateNiagara();
		ProjectileNiagaraEffectActor = nullptr;
	}

	if (HasAuthority())
	{
		if (
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary
				::GetAbilitySystemComponent(OtherActor)
		)
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}
