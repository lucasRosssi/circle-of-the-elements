// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/BeamAbility.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/UtilityLibrary.h"

void UBeamAbility::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (!HitResult.bBlockingHit) return;

  AActor* AvatarActor = GetAvatarActorFromActorInfo();
  const FVector SocketLocation = ICombatInterface::Execute_GetAbilitySocketLocation(
    AvatarActor,
    AbilitySocketName,
    bUseWeaponSocket
    );

  const float AttemptedDistance = FVector::Distance(SocketLocation, HitResult.ImpactPoint);
  const float CurrentBeamRange = GetBeamRange();
  
  if (AttemptedDistance > CurrentBeamRange)
  {
    FVector Direction = HitResult.ImpactPoint - SocketLocation;
    Direction.Normalize();
    MouseHitLocation = SocketLocation + Direction * CurrentBeamRange;
    MouseHitActor = nullptr;

    if (bDebugAbility)
    {
      DrawDebugDirectionalArrow(
        GetWorld(),
        MouseHitLocation,
        HitResult.ImpactPoint,
        1000.f,
        FColor::Red,
        false,
        DrawShapeDuration,
        0,
        3.f
      );
    }
  }
  else
  {
    MouseHitLocation = HitResult.ImpactPoint;
	  MouseHitActor = HitResult.GetActor();
  }

  if (bDebugAbility)
  {
    DrawDebugDirectionalArrow(
      GetWorld(),
      SocketLocation,
      MouseHitLocation,
      1000.f,
      FColor::Green,
      false,
      DrawShapeDuration,
      0,
      3.f
    );
  }
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
		UUtilityLibrary::GetDistance(
		  SocketLocation,
		  BeamTargetLocation
		  ),
		UUtilityLibrary::GetMiddlePoint(
		  SocketLocation,
		  BeamTargetLocation
		  )
		);

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(
		AvatarActor,
		SocketLocation,
		BeamTargetLocation,
		GetBeamTraceRadius(),
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

float UBeamAbility::GetBeamTraceRadius() const
{
  return BeamTraceRadius + AdditionalBeamTraceRadius;
}

float UBeamAbility::GetBeamTickPeriod() const
{
  return BeamTickPeriod + AdditionalBeamTickPeriod;
}

float UBeamAbility::GetBeamRange() const
{
  return BeamRange.GetValueAtLevel(GetAbilityLevel()) + AdditionalBeamRange;
}
