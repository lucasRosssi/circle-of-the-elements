// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/AreaEffectActorAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AreaEffectActor.h"
#include "Aura/Aura.h"

UAreaEffectActorAbility::UAreaEffectActorAbility()
{
	bIsAreaAbility = true;
}

int32 UAreaEffectActorAbility::GetAreaEffectDurationAtLevel_Implementation(int32 Level) const
{
	return FMath::RoundToInt32(AreaEffectDuration.GetValueAtLevel(Level));
}

float UAreaEffectActorAbility::GetPeriodAtLevel_Implementation(int32 Level) const
{
	return Period.GetValueAtLevel(Level);
}

bool UAreaEffectActorAbility::IsAreaEffectActorAbility_Implementation() const
{
	return true;
}

float UAreaEffectActorAbility::GetAreaEffectDuration() const
{
  return AreaEffectDuration.GetValueAtLevel(GetAbilityLevel()) + AdditionalAreaEffectDuration;
}

float UAreaEffectActorAbility::GetPeriod() const
{
  return Period.GetValueAtLevel(GetAbilityLevel()) + AdditionalPeriod;
}

AAreaEffectActor* UAreaEffectActorAbility::SpawnEffectActor(const FVector& TargetLocation)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor->HasAuthority()) return nullptr;
	
	FTransform SpawnTransform;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TargetLocation + FVector(0.f, 0.f, 400.f),
		TargetLocation - FVector(0.f, 0.f, 400.f),
		ECC_ExcludeCharacters
	);

  FVector SpawnPoint;
	if (HitResult.bBlockingHit)
	{
	  SpawnPoint = HitResult.ImpactPoint;
	}
	else
	{
	  SpawnPoint = TargetLocation;
	}
	
	SpawnTransform.SetLocation(SpawnPoint + SpawnLocationOffset);
	SpawnTransform.SetRotation(AvatarActor->GetActorRotation().Quaternion());
	
 	AAreaEffectActor* AreaEffectActor = GetWorld()->SpawnActorDeferred<AAreaEffectActor>(
			 AreaEffectClass,
			 SpawnTransform,
			 GetOwningActorFromActorInfo(),
			 Cast<APawn>(AvatarActor),
			 ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		 );

	UAuraAbilitySystemLibrary::JoinToActorTeam(
		GetAvatarActorFromActorInfo(),
		AreaEffectActor,
		true
		);
 	AreaEffectActor->TargetTeam = AbilityTargetTeam;
 	AreaEffectActor->AbilityParams = MakeAbilityParamsFromDefaults();
	AreaEffectActor->SetActorDuration(GetAreaEffectDuration());
	AreaEffectActor->bInstant = bInstant;
	AreaEffectActor->Period = GetPeriod();
	AreaEffectActor->DelayImpact = DelayImpact.GetValueAtLevel(GetAbilityLevel());
  AreaEffectActor->bHasRadialForce = bHasRadialForce;
  AreaEffectActor->Force = Force.GetValueAtLevel(GetAbilityLevel());
  AreaEffectActor->bActorUsesCharges = bActorUsesCharges;
  AreaEffectActor->ActorCharges = GetActorCharges();
  AreaEffectActor->SpawnPointCache = SpawnTransform.GetLocation();
 	AreaEffectActor->FinishSpawning(SpawnTransform);

	return AreaEffectActor;
}

