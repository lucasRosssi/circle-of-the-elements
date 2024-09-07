// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/MeleeAttackAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Enums/MeleeHitMode.h"
#include "Interaction/CombatInterface.h"

void UMeleeAttackAbility::ExecuteAttackScan()
{
	const FVector AbilitySocketLocation = ICombatInterface::Execute_GetAbilitySocketLocation(
		GetAvatarActorFromActorInfo(),
		AbilitySocketName,
		bUseWeaponSocket
		);

	if (bDebugAbility)
	{
		DrawDebugMeleeHitShape(AbilitySocketLocation);
	}

	const TArray<AActor*>& HitActors = ShapeScan(AbilitySocketLocation);

	for (const auto HitActor : HitActors)
	{
		ApplyEffectToHitActor(HitActor);
	}
}

void UMeleeAttackAbility::HandleWeaponCollision()
{
	UBoxComponent* WeaponBox = ICombatInterface::Execute_EnableWeaponCollision(GetAvatarActorFromActorInfo(), true);
	WeaponBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &UMeleeAttackAbility::OnWeaponCollision);
}

void UMeleeAttackAbility::DrawDebugMeleeHitShape(const FVector& AbilitySocketLocation)
{
	switch (MeleeHitMode)
	{
	case EMeleeHitMode::Sphere:
		{
			DrawDebugSphere(
				GetWorld(),
				AbilitySocketLocation,
				SphereHitRadius,
				12,
				DrawShapeColor,
				false,
				DrawShapeDuration
			);
			break;
		}
	case EMeleeHitMode::Box:
		{
			DrawDebugBox(
				GetWorld(),
				AbilitySocketLocation,
				BoxHitDimensions,
				DrawShapeColor,
				false,
				DrawShapeDuration
			);
			break;
		}
	case EMeleeHitMode::Weapon: {}
	default: {}
	}
}

TArray<AActor*> UMeleeAttackAbility::ShapeScan(const FVector& Origin)
{
	TArray ActorsToIgnore({ GetAvatarActorFromActorInfo() });
	TArray<AActor*> AliveActors;
	switch (MeleeHitMode)
	{
	case EMeleeHitMode::Sphere:
		{
			UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
				GetAvatarActorFromActorInfo(),
				AliveActors,
				ActorsToIgnore,
				SphereHitRadius,
				Origin,
				AbilityTargetTeam
			);
			break;
		}
	case EMeleeHitMode::Box:
		{
			UAuraAbilitySystemLibrary::GetAliveCharactersWithinBox(
				GetAvatarActorFromActorInfo(),
				AliveActors,
				ActorsToIgnore,
				BoxHitDimensions,
				Origin,
				AbilityTargetTeam
			);
			break;
		}
	case EMeleeHitMode::Weapon: {}
	default: {}
	}

	return AliveActors;
}

void UMeleeAttackAbility::ApplyEffectToHitActor(AActor* HitActor)
{
	FAbilityParams AbilityParams = MakeAbilityParamsFromDefaults(HitActor);
	AbilityParams.ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
		
	bool bSuccess;
	UAuraAbilitySystemLibrary::ApplyAbilityEffect(AbilityParams, bSuccess);

	if (bDebugAbility)
	{
		DrawDebugSphere(
			GetWorld(),
			HitActor->GetActorLocation(),
			70.f,
			12,
			FColor::Red,
			false,
			DrawShapeDuration
			);
	}
}

void UMeleeAttackAbility::OnWeaponCollision(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (GetAvatarActorFromActorInfo() == OtherActor) return;

	bool bValidTarget = false;
	switch (AbilityTargetTeam)
	{
	case ETargetTeam::Enemies:
		{
			bValidTarget = UAuraAbilitySystemLibrary::AreActorsEnemies(GetAvatarActorFromActorInfo(), OtherActor);
			break;
		}
	case ETargetTeam::Friends:
		{
			bValidTarget = UAuraAbilitySystemLibrary::AreActorsFriends(GetAvatarActorFromActorInfo(), OtherActor);
			break;
		}
	case ETargetTeam::Both:
		{
			bValidTarget = true;
			break;
		}
	default: {}
	}

	if (bValidTarget)	ApplyEffectToHitActor(OtherActor);
}
