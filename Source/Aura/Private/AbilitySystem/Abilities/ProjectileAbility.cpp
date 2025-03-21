// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "Interfaces/CombatInterface.h"

void UProjectileAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
	)
{
	bScannedForTargets = false;
	Targets.Empty();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UProjectileAbility::SpawnProjectile(
	const FVector& ProjectileTargetLocation,
	const FName SocketName,
	bool bSocketInWeapon
)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor->HasAuthority()) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetAbilitySocketLocation(
		AvatarActor,
		SocketName,
		bSocketInWeapon
	);
	FRotator Rotation = AvatarActor->GetActorRotation();
	Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(
		Forward,
		FVector::UpVector,
		ProjectileSpread, 
		ProjectileCount.AsInteger(GetAbilityLevel())
		);

	for (const auto& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
	  FRotator FinalRotator = Rotator;
	  if (ProjectileAimSpread > 0.f)
	  {
	    const float RandomDeviation = FMath::FRandRange(-ProjectileAimSpread / 2, ProjectileAimSpread / 2);
	    FinalRotator.Yaw = FinalRotator.Yaw + RandomDeviation;
	  }
		SpawnTransform.SetRotation(FinalRotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(AvatarActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (bHoming)
		{
			if (HomingMode != EProjectileHomingMode::OneTarget && !bScannedForTargets)
			{
				TArray<AActor*> ActorsToIgnore;
				UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
					GetAvatarActorFromActorInfo(),
					Targets,
					ActorsToIgnore,
					Projectile->GetHomingSphere()->GetScaledSphereRadius(),
					SpawnTransform.GetLocation(),
					ETargetTeam::Enemies
					);
				
				bScannedForTargets = true;
			}

			if (!Targets.IsEmpty())
			{
				SetHomingInitialTarget(Projectile);
			}
			
			if (HomingActivationDelay > 0.f)
			{
				Projectile->ScheduleHomingActivation(HomingActivationDelay);
			}
			else
			{
				Projectile->ActivateHomingMode();
			}
		}

		if (RangedHitMode != EAbilityHitMode::Default)
		{
			Projectile->HitMode = RangedHitMode;
			Projectile->MaxHitCount = GetMaxHitCount();
			Projectile->EffectChangePerHit = GetEffectChangePerHit();
			Projectile->bCanRepeatTarget = bCanRepeatTarget;
		}

		Projectile->TargetTeam = AbilityTargetTeam;
		Projectile->AbilityParams = MakeAbilityParamsFromDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UProjectileAbility::SetHomingInitialTarget(AAuraProjectile* Projectile)
{
	switch (HomingMode)
	{
	case EProjectileHomingMode::DifferentTargets:
		{
			if (Targets.IsValidIndex(ProjectileSpawnCycleCount))
			{
				Projectile->InitialTarget = Targets[ProjectileSpawnCycleCount];
			}
			else
			{
				ProjectileSpawnCycleCount = 0;
				Projectile->InitialTarget = Targets[ProjectileSpawnCycleCount];
			}
			ProjectileSpawnCycleCount += 1;
			break;
		}
	case EProjectileHomingMode::RandomTargets:
		{
			const int32 RandomIndex = FMath::RandRange(0, Targets.Num() - 1);
			Projectile->InitialTarget = Targets[RandomIndex];
			break;
		}
	case EProjectileHomingMode::OneTarget: {}
	default: {}
	}
}
