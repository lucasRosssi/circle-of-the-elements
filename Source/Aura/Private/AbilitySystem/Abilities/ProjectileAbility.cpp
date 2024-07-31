// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

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
	ProjectileCount
	);

	for (const auto& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(AvatarActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (bHoming)
		{
			if (HomingActivationDelay > 0.f)
			{
				Projectile->ScheduleHomingActivation(HomingActivationDelay);
			}
			else
			{
				Projectile->ActivateHomingMode();
			}
		}

		if (HitMode != EAbilityHitMode::Default)
		{
			Projectile->HitMode = HitMode;
			Projectile->MaxHitCount = GetMaxHitCount();
			Projectile->EffectChangePerHit = GetEffectChangePerHit();
			Projectile->bCanRepeatTarget = bCanRepeatTarget;
		}

		Projectile->TargetTeam = AbilityTargetTeam;
		Projectile->AbilityParams = MakeAbilityParamsFromDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}
