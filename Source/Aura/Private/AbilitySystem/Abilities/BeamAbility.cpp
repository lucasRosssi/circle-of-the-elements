// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/BeamAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/UtilityLibrary.h"

void UBeamAbility::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit) return;

	MouseHitLocation = HitResult.ImpactPoint;
	MouseHitActor = HitResult.GetActor();
}

USceneComponent* UBeamAbility::GetComponentToAttachGameplayCue()
{
	if (bUseWeaponSocket)
	{
		return ICombatInterface::Execute_GetWeapon(GetAvatarActorFromActorInfo());
	}

	return ICombatInterface::Execute_GetAvatarMesh(GetAvatarActorFromActorInfo());
}

void UBeamAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	check(AvatarActor);
	
	const FVector SocketLocation = ICombatInterface::Execute_GetAbilitySocketLocation(
		AvatarActor,
		AbilitySocketName,
		bUseWeaponSocket
		);
	
	TArray ActorsToIgnore({
		AvatarActor
	});
	UAuraAbilitySystemLibrary::GetFriendsWithinRadius(
		AvatarActor,
		ActorsToIgnore,
		UUtilityLibrary::GetDistance(SocketLocation, BeamTargetLocation),
		UUtilityLibrary::GetMiddlePoint(SocketLocation, BeamTargetLocation)
		);

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(
		AvatarActor,
		SocketLocation,
		BeamTargetLocation,
		BeamTraceRadius,
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
		);
	

	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
}
