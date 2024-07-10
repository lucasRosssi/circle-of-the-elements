// Copyright Lucas Rossi


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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

void AAuraProjectile::OnHit(bool bDeactivateEffect)
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

	if (ProjectileNiagaraEffectActor && bDeactivateEffect)
	{
		ProjectileNiagaraEffectActor->DeactivateNiagara();
		ProjectileNiagaraEffectActor = nullptr;
	}
}

void AAuraProjectile::Destroyed()
{
	if (!HasAuthority())	OnHit();

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

void AAuraProjectile::HandleAreaAbility(AActor* OtherActor, const AActor* EffectCauser, bool bSuccess)
{
	AbilityParams.AreaOrigin = GetActorLocation();
	AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
		AbilityParams.AreaOrigin, 
		OtherActor->GetActorLocation()
	);

	TArray<AActor*> TargetsInArea;
	TArray<AActor*> IgnoreActors;
	UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
		EffectCauser,
		TargetsInArea,
		IgnoreActors,
		AbilityParams.AreaOuterRadius,
		AbilityParams.AreaOrigin,
		TargetTeam
	);

	for (const auto Target : TargetsInArea)
	{
		if (
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary
				::GetAbilitySystemComponent(Target)
		)
		{
			AbilityParams.TargetASC = TargetASC;
			ApplyProjectileEffect(bSuccess);
		}
	}
}

void AAuraProjectile::HandleSingleTarget(AActor* OtherActor, bool bSuccess)
{
	if (
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary
			::GetAbilitySystemComponent(OtherActor)
	)
	{
		AbilityParams.ForwardVector = GetActorForwardVector();
		AbilityParams.TargetASC = TargetASC;
				
		ApplyProjectileEffect(bSuccess);
	}
}

void AAuraProjectile::HandleBounceHitMode(AActor* OtherActor)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Append(ActorsHit);
	AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		OtherActor,
		BounceRadius,
		TargetTeam == ETargetTeam::Both ? TargetTeam : ETargetTeam::Friends,
		ActorsToIgnore
	);

	if (ClosestActor)
	{
		OnHit(false);
		HitCount++;
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(),
			ClosestActor->GetActorLocation()
		));
		ActorsHit.Add(OtherActor);
		ProjectileMovement->SetVelocityInLocalSpace(GetVelocity());
		// If it is also homing, set the homing target to the closest actor
		if (ProjectileMovement->bIsHomingProjectile)
		{
			MulticastHomingTarget(ClosestActor);
		}
	}
	// No actor close
	else
	{
		Destroy();
	}
}

void AAuraProjectile::HandlePenetrationHitMode(AActor* OtherActor)
{
	if (OtherActor->Implements<UCombatInterface>())
	{
		OnHit(false);
		HitCount++;
		ActorsHit.Add(OtherActor);
		// If it is also homing, we should set a new target after penetrating
		if (ProjectileMovement->bIsHomingProjectile)
		{
			TArray<AActor*> ActorsToIgnore;
			AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
				OtherActor,
				BounceRadius,
				TargetTeam == ETargetTeam::Both ? TargetTeam : ETargetTeam::Friends,
				ActorsToIgnore
			);
			MulticastHomingTarget(ClosestActor);
		}
	}
	// Hit the environment
	else
	{
		Destroy();
	}
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
	if (ActorsHit.Contains(OtherActor)) return;
	const AActor* EffectCauser = AbilityParams.SourceASC->GetAvatarActor();
	
	if (
		// OtherActor is the instigator
		EffectCauser == OtherActor ||
		// OtherActor is friend
		UAuraAbilitySystemLibrary::AreActorsFriends(EffectCauser, OtherActor)
	)
		return;

	if (UAuraAbilitySystemLibrary::IsTargetInvulnerable(OtherActor)) return;

	OnHit(
		HitMode == EAbilityHitMode::Default ||
		HitCount >= MaxHitCount
		);
	
	if (HasAuthority())
	{
		bool bSuccess = false;
		/*
		 * If the ability affects an area we should apply the effect in everyone
		 * within radius
		 */
		if (AbilityParams.bIsAreaAbility) HandleAreaAbility(OtherActor, EffectCauser, bSuccess);
		/*
		 * If it is a single target, we just have to apply the effect to the actor hit
		 */
		else HandleSingleTarget(OtherActor, bSuccess);
		/*
		 * If it is a bouncing ability, we have to find the closest actor to bounce to.
		 * If there is no actor close, we destroy the projectile.
		 */
		if (HitMode == EAbilityHitMode::Bounce && HitCount + 1 < MaxHitCount)
		{
			HandleBounceHitMode(OtherActor);
		}
		/*
		 * If it is a penetrating ability, we just have to check if the target hit implements
		 * the combat interface and keep the projectile trajectory. If if doesn't implement it,
		 * then the projectile hit the environment and should destroy itself
		 */
		else if (HitMode == EAbilityHitMode::Penetration && HitCount + 1 < MaxHitCount)
		{
			HandlePenetrationHitMode(OtherActor);
		}
		/*
		 * Has a default hit mode or reached maximum bounce/penetration hits
		 */
		else
		{
			Destroy();
		}
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
			ETargetTeam::Friends,
			ActorsToIgnore
			);
		
		MulticastHomingTarget(ClosestActor);
	}
}

void AAuraProjectile::ApplyProjectileEffect(bool& bSuccess)
{
	if (HitMode != EAbilityHitMode::Default && MaxHitCount > 1)
	{
		AbilityParams.DamageParams.BaseDamage *= (1.f + EffectChangePerHit * HitCount);
		AbilityParams.EffectParams.Value *= (1.f + EffectChangePerHit * HitCount);
		AbilityParams.EffectParams.Duration *= (1.f + EffectChangePerHit * HitCount);
	}
	
	UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
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

