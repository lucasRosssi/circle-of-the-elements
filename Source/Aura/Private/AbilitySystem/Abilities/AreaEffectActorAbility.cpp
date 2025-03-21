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

	if (HitResult.bBlockingHit)
	{
		SpawnTransform.SetLocation(HitResult.ImpactPoint);
	}
	else
	{
		SpawnTransform.SetLocation(TargetLocation);
	}
	
	SpawnTransform.SetRotation(AvatarActor->GetActorRotation().Quaternion());
	
 	AAreaEffectActor* AreaEffectActor = GetWorld()->SpawnActorDeferred<AAreaEffectActor>(
			 AreaEffectClass,
			 SpawnTransform,
			 GetOwningActorFromActorInfo(),
			 Cast<APawn>(AvatarActor),
			 ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		 );

	UAuraAbilitySystemLibrary::JoinToActorTeam(
		GetAvatarActorFromActorInfo(),
		AreaEffectActor,
		true
		);
 	AreaEffectActor->TargetTeam = AbilityTargetTeam;
 	AreaEffectActor->AbilityParams = MakeAbilityParamsFromDefaults();
	AreaEffectActor->SetActorDuration(AreaEffectDuration.GetValueAtLevel(GetAbilityLevel()));
	AreaEffectActor->bInstant = bInstant;
	AreaEffectActor->Period = Period.GetValueAtLevel(GetAbilityLevel());
	AreaEffectActor->DelayImpact = DelayImpact.GetValueAtLevel(GetAbilityLevel());
  AreaEffectActor->bHasRadialForce = bHasRadialForce;
  AreaEffectActor->Force = Force.GetValueAtLevel(GetAbilityLevel());
  AreaEffectActor->bActorUsesCharges = bActorUsesCharges;
  AreaEffectActor->ActorCharges = GetActorCharges();
 	AreaEffectActor->FinishSpawning(SpawnTransform);

	return AreaEffectActor;
}

