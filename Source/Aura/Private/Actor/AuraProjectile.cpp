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
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

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

	HomingRadius = CreateDefaultSubobject<USphereComponent>("HomingRadius");
	HomingRadius->SetupAttachment(GetRootComponent());
	HomingRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HomingRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	HomingRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::ActivateHomingMode()
{
	ProjectileMovement->bIsHomingProjectile = true;

	HomingRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	HomingRadius->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnEnteringHomingRadius);

	if (ProjectileNiagaraEffect)
	{
		ProjectileNiagaraEffectActor = GetWorld()->SpawnActor<AProjectileEffect>(
			ProjectileNiagaraEffect,
			GetActorLocation(),
			GetActorRotation()
		);

		const FAttachmentTransformRules Rules = FAttachmentTransformRules(
			EAttachmentRule::KeepWorld,
			false
		);
		ProjectileNiagaraEffectActor->AttachToActor(this, Rules);
	}
	
	if (MuzzleEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			MuzzleEffect,
			GetActorLocation(),
			GetActorRotation()
		);
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

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation(),
		FRotator::ZeroRotator,
		ImpactSoundVolume,
		ImpactSoundPitch
	);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ImpactEffect,
		GetActorLocation(),
		GetActorRotation()
	);

	if (ProjectileNiagaraEffectActor)
	{
		ProjectileNiagaraEffectActor->DeactivateNiagara();
		ProjectileNiagaraEffectActor = nullptr;
	}
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())	OnHit();

	if (ProjectileNiagaraEffectActor)
	{
		ProjectileNiagaraEffectActor->DeactivateNiagara();
		ProjectileNiagaraEffectActor = nullptr;
	}
	Super::Destroyed();
}

void AAuraProjectile::ScheduleHomingActivation(float Delay)
{
	FTimerHandle HomingTimer;
	GetWorld()->GetTimerManager().SetTimer(
		HomingTimer,
		this,
		&AAuraProjectile::ActivateHomingMode,
		Delay,
		false
	);
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
	if (!IsValid(AbilityParams.SourceASC)) return;
	const AActor* EffectCauser = AbilityParams.SourceASC->GetAvatarActor();
	
	if (
		// OtherActor is the instigator
		EffectCauser == OtherActor ||
		// OtherActor is friend
		UAuraAbilitySystemLibrary::AreActorsFriends(EffectCauser, OtherActor)
	)
		return;

	if (UAuraAbilitySystemLibrary::IsTargetInvulnerable(OtherActor)) return;
	
	OnHit();

	if (HasAuthority())
	{
		bool bSuccess;
		if (AbilityParams.bIsAreaAbility)
		{
			AbilityParams.AreaOrigin = GetActorLocation();
			AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
				AbilityParams.AreaOrigin, 
				OtherActor->GetActorLocation()
				);

			TArray<AActor*> EnemiesInArea;
			TArray<AActor*> IgnoreActors;
			UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
				EffectCauser,
				EnemiesInArea,
				IgnoreActors,
				AbilityParams.AreaOuterRadius,
				AbilityParams.AreaOrigin,
				ETargetTeam::ETT_Enemies
				);

			for (const auto Enemy : EnemiesInArea)
			{
				if (
					UAbilitySystemComponent* EnemyASC = UAbilitySystemBlueprintLibrary
						::GetAbilitySystemComponent(Enemy)
				)
				{
					AbilityParams.TargetASC = EnemyASC;
					UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
				}
			}
		}
		else if (
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary
				::GetAbilitySystemComponent(OtherActor)
		)
		{
			AbilityParams.ForwardVector = GetActorForwardVector();
			AbilityParams.TargetASC = TargetASC;
				
			UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AAuraProjectile::OnEnteringHomingRadius(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	const AActor* EffectCauser = AbilityParams.SourceASC->GetAvatarActor();
	
	if (
		// OtherActor is the instigator
		EffectCauser == OtherActor ||
		// Already has a target
		IsValid(HomingTarget) ||
		// OtherActor is friend
		UAuraAbilitySystemLibrary::AreActorsFriends(EffectCauser, OtherActor)
	)
		return;

	SetReplicateMovement(true);
	MulticastHomingTarget(OtherActor);
}

void AAuraProjectile::OnHomingTargetDied(AActor* DeadActor)
{
	if (IsValid(DeadActor))
	{
		if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(DeadActor))
		{
			CombatTarget->GetOnDeathDelegate().RemoveDynamic(this, &AAuraProjectile::OnHomingTargetDied);
		}
		
		TArray<AActor*> ActorsToIgnore;
		AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
			DeadActor,
			HomingRadius->GetUnscaledSphereRadius(),
			ETargetTeam::ETT_Friends,
			ActorsToIgnore
			);
		
		MulticastHomingTarget(ClosestActor);
	}
}

void AAuraProjectile::MulticastHomingTarget_Implementation(AActor* Target)
{
	if (!IsValid(Target))
	{
		HomingTarget = nullptr;
		ProjectileMovement->HomingTargetComponent = nullptr;
		ProjectileMovement->bRotationFollowsVelocity = false;
		return;
	}
	
	HomingTarget = Target;
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	ProjectileMovement->bRotationFollowsVelocity = true;

	if (!HasAuthority()) return;
	if (ICombatInterface* CombatTarget = Cast<ICombatInterface>(Target))
	{
		CombatTarget->GetOnDeathDelegate().AddDynamic(this, &AAuraProjectile::OnHomingTargetDied);
	}
}

