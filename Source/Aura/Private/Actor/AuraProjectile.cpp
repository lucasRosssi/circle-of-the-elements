// Copyright Lucas Rossi


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayCueFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AuraProjectileMovementComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/PlayerInterface.h"
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

  ProjectileNiagara = CreateDefaultSubobject<UNiagaraComponent>("ProjectileNiagara");
  ProjectileNiagara->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UAuraProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

  InitialLifeSpan = 0.f;
}

void AAuraProjectile::ActivateHomingMode()
{
	ProjectileMovement->bIsHomingProjectile = true;
	if (IsValid(InitialTarget) && !ICombatInterface::Execute_IsDead(InitialTarget))
	{
		SetReplicateMovement(true);
		MulticastHomingTarget(InitialTarget);
	}
	
	HomingRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

  if (ProjectileDuration > 0)
  {
    GetWorldTimerManager().SetTimer(
      LifeSpanTimer,
      this,
      &AAuraProjectile::PrepareDestroy,
      ProjectileDuration,
      false
    );
  }
  
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	HomingRadius->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnEnteringHomingRadius);
	
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

  if (bYoYoReturnCanRepeatTarget)
  {
    ProjectileMovement->OnYoYoReturnDelegate.AddLambda([this]()
      {
        ActorsHit.Empty();  
      }
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

	if (bDeactivateEffect)
	{
		ProjectileNiagara->Deactivate();
	}
}

void AAuraProjectile::Destroyed()
{
	if (!HasAuthority()) OnHit();

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

void AAuraProjectile::HandleAreaAbility(const AActor* EffectCauser, bool& bSuccess)
{
	AbilityParams.AreaOrigin = GetActorLocation();

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
			AbilityParams.ForwardVector = UKismetMathLibrary::GetDirectionUnitVector(
				AbilityParams.AreaOrigin, 
				Target->GetActorLocation()
				);
			ApplyProjectileEffect(bSuccess);
		}
	}
}

void AAuraProjectile::HandleSingleTarget(AActor* OtherActor, bool& bSuccess)
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

void AAuraProjectile::HandleRicochetHitMode(AActor* OtherActor)
{
	AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		OtherActor,
		RicochetRadius,
		TargetTeam == ETargetTeam::Both ? TargetTeam : ETargetTeam::Friends,
		ActorsHit
	);

	if (ClosestActor)
	{
		OnHit(false);
		HitCount++;
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(),
			ClosestActor->GetActorLocation()
		));
		if (!bCanRepeatTarget)	ActorsHit.Add(OtherActor);
		
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
		PrepareDestroy();
	}
}

void AAuraProjectile::HandlePenetrationHitMode(AActor* OtherActor)
{
	if (OtherActor->Implements<UCombatInterface>())
	{
		OnHit(false);
		HitCount++;
	  if (!bCanRepeatTarget)
	  {
		  ActorsHit.Add(OtherActor);
	  }
		// If it is also homing, we should set a new target after penetrating
		if (ProjectileMovement->bIsHomingProjectile)
		{
			AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
				OtherActor,
				RicochetRadius,
				TargetTeam == ETargetTeam::Both ? TargetTeam : ETargetTeam::Friends,
				ActorsHit
			);
			MulticastHomingTarget(ClosestActor);
		}
	}
	// Hit the environment
	else
	{
		PrepareDestroy();
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
	
	if (HitCount == 0 && EffectCauser == OtherActor) return;

	bool bSuccess = false;
	
	if (UAuraAbilitySystemLibrary::IsEnvironment(OtherActor))
	{
		OnHit(true);
		if (AbilityParams.bIsAreaAbility) HandleAreaAbility(EffectCauser, bSuccess);
		if (HasAuthority()) PrepareDestroy();
		return;
	}
	if (
		TargetTeam == ETargetTeam::Enemies &&
		!UAuraAbilitySystemLibrary::AreActorsEnemies(EffectCauser, OtherActor)
	)	return;
	if (
		TargetTeam == ETargetTeam::Friends &&
		!UAuraAbilitySystemLibrary::AreActorsFriends(EffectCauser, OtherActor)
	)	return;

	if (UAuraAbilitySystemLibrary::IsTargetInvulnerable(OtherActor)) return;

	OnHit(
		HitMode == EAbilityHitMode::Default ||
		HitCount >= MaxHitCount
		);
	
	if (HasAuthority())
	{
		/*
		 * If the ability affects an area we should apply the effect in everyone
		 * within radius
		 */
		if (AbilityParams.bIsAreaAbility) HandleAreaAbility(EffectCauser, bSuccess);
		/*
		 * If it is a single target, we just have to apply the effect to the actor hit
		 */
		else HandleSingleTarget(OtherActor, bSuccess);
		/*
		 * If it is a bouncing ability, we have to find the closest actor to ricochet to.
		 * If there is no actor close, we destroy the projectile.
		 */
		if (HitMode == EAbilityHitMode::Ricochet && HitCount + 1 < MaxHitCount)
		{
			HandleRicochetHitMode(OtherActor);
		}
		/*
		 * If it is a penetrating ability, we just have to check if the target hit implements
		 * the combat interface and keep the projectile trajectory. If it doesn't implement it,
		 * then the projectile hit the environment and should destroy itself
		 */
		else if (HitMode == EAbilityHitMode::Penetration && HitCount + 1 < MaxHitCount)
		{
			HandlePenetrationHitMode(OtherActor);
		}
		/*
		 * Has a default hit mode or reached maximum ricochet/penetration hits
		 */
		else
		{
			PrepareDestroy();
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
	  if (AbilityParams.EffectParams.Num() > 0)
	  {
	    for (auto& StatusEffect : AbilityParams.EffectParams)
	    {
	      StatusEffect.Value *= (1.f + EffectChangePerHit * HitCount);
	      StatusEffect.Duration *= (1.f + EffectChangePerHit * HitCount);
	    }
	  }
	}
	
	UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);
}

void AAuraProjectile::PrepareDestroy()
{
  if (IsValid(ProjectileNiagara))
  {
    ProjectileNiagara->Deactivate();
    if (IsValid(Sphere))
    {
      Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      Sphere->Deactivate();
    }
    if (IsValid(HomingRadius))
    {
      HomingRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
      HomingRadius->Deactivate();
    }
    if (IsValid(ProjectileMovement))
    {
      ProjectileMovement->Velocity = FVector::ZeroVector;
    }
    
    UStaticMeshComponent* Mesh = GetComponentByClass<UStaticMeshComponent>();
    if (Mesh)
    {
      Mesh->SetHiddenInGame(true);
    }
    
    FTimerHandle DestroyTimer;
    GetWorldTimerManager().SetTimer(
      DestroyTimer,
      this,
      &AAuraProjectile::CallDestroy,
      ParticlesLifecycleTimeBeforeDestroy,
      false
    );

    return;
  }

  Destroy();
}

void AAuraProjectile::CallDestroy()
{
  Destroy();
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
		CombatTarget->GetOnDeathDelegate().AddUniqueDynamic(this, &AAuraProjectile::OnHomingTargetDied);
	}
}

