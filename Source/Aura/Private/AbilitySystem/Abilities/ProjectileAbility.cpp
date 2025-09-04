// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/StatusEffectInfo.h"
#include "Actor/AuraProjectile.h"
#include "Components/AuraProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Utils/AuraSystemsLibrary.h"

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

int32 UProjectileAbility::GetProjectileCount() const
{
  return ProjectileCount.AsInteger(GetAbilityLevel()) + AdditionalProjectileCount;
}

float UProjectileAbility::GetProjectileSpread() const
{
  return ProjectileSpread + AdditionalProjectileSpread;
}

float UProjectileAbility::GetProjectileSpacing() const
{
  return ProjectileSpacing + AdditionalProjectileSpacing;
}

TArray<FTransform> UProjectileAbility::HandleParallelProjectiles(
  const FVector& SocketLocation,
  const FRotator& Rotation,
  const FVector& Forward
)
{
  TArray<FVector> SpawnPoints = UAuraAbilitySystemLibrary::EvenlySpacedPoints(
    SocketLocation,
    Forward,
    FVector::UpVector,
    GetProjectileSpacing(),
    GetProjectileCount()
  );

  TArray<FTransform> SpawnTransforms;
  for (const FVector& Location : SpawnPoints)
  {
    FTransform SpawnTransform;
    SpawnTransform.SetLocation(Location);
    SpawnTransform.SetRotation(Rotation.Quaternion());

    SpawnTransforms.Add(SpawnTransform);
  }

  return SpawnTransforms;
}

TArray<FTransform> UProjectileAbility::HandleArcProjectiles(const FVector& SocketLocation, const FVector& Forward)
{
  TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(
    Forward,
    FVector::UpVector,
    GetProjectileSpread(), 
    GetProjectileCount()
  );

  TArray<FTransform> SpawnTransforms;
  for (const FRotator& Rotator : Rotators)
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

    SpawnTransforms.Add(SpawnTransform);
  }

  return SpawnTransforms;
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
	const FVector& Forward = Rotation.Vector();
  
  TArray<FTransform> SpawnTransforms;
  if (GetProjectileSpread() < 15.f && GetProjectileCount() > 1)
  {
    SpawnTransforms = HandleParallelProjectiles(SocketLocation, Rotation, Forward);
  }
  else
  {
	  SpawnTransforms = HandleArcProjectiles(SocketLocation, Forward);
  }

  for (const FTransform& SpawnTransform : SpawnTransforms)
  {
    AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
      OverrideProjectileClass ? OverrideProjectileClass : ProjectileClass,
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

    if (ProjectileMotionType != EProjectileMotionType::Default)
    {
      Projectile->ProjectileMovement->SetActiveMotion(ProjectileMotionType);
    }

    if (bMotionDirectionAlternates)
    {
      Projectile->ProjectileMovement->bDefinedDirections = true;
      Projectile->ProjectileMovement->DirectionMultiplier = ProjectileDirectionShift;
      ProjectileDirectionShift *= -1.f;
    }

    if (bUseTargetLocationAsBezierFinalLocation)
    {
      Projectile->ProjectileMovement->bBezierOverride = true;
      const FTransform& StartTransform = Projectile->ProjectileMovement->UpdatedComponent->GetComponentTransform();
      const FVector LocalOffset = StartTransform.InverseTransformPosition(ProjectileTargetLocation);
      Projectile->ProjectileMovement->BezierEndOverride = LocalOffset;
    }

    if (bYoYoReturnToAvatar)
    {
      Projectile->ProjectileMovement->bYoYoReturnToAvatar = true;
      Projectile->ProjectileMovement->AvatarActor = AvatarActor;
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

    if (bApplyElementalFlowNiagara)
    {
      const UStatusEffectInfo* Info = UAuraSystemsLibrary::GetStatusEffectInfo(GetAvatarActorFromActorInfo());
      const FStatusEffectData* Data = Info->StatusEffects.Find(GetCurrentElementalFlowTag());
      
      if (Data)
      {
        UNiagaraFunctionLibrary::SpawnSystemAttached(
          Data->NiagaraSystem,
          Projectile->GetRootComponent(),
          FName(),
          FVector(0),
          FRotator(0),
          EAttachLocation::KeepRelativeOffset,
          true
        );
      }
    }
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
