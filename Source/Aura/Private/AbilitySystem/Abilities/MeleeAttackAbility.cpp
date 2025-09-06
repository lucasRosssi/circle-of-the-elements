// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/MeleeAttackAbility.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Enums/MeleeHitMode.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/PlayerInterface.h"

#if WITH_EDITOR
void UMeleeAttackAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  Super::PostEditChangeProperty(PropertyChangedEvent);

  // Get the name of the property that was changed
  const FName PropertyName = PropertyChangedEvent.Property ?
    PropertyChangedEvent.Property->GetFName() : NAME_None;

  // Check the changed property
  if (PropertyName == GET_MEMBER_NAME_CHECKED(UMeleeAttackAbility, ComboSequenceData))
  {
    if (ComboSequenceData.Num() > 0)
    {
      ComboSequenceData[ComboSequenceData.Num() - 1].bMeleeAbility = true;
    }
  }
}
#endif

void UMeleeAttackAbility::EndAbility(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  bool bReplicateEndAbility,
  bool bWasCancelled
  )
{
  Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
  
  ActorsHitInMontage.Empty();
}

void UMeleeAttackAbility::HandleComboSequence()
{
  ActorsHitInMontage.Empty();
  Super::HandleComboSequence();
}

void UMeleeAttackAbility::ExecuteAttackScan()
{
	const FVector AbilitySocketLocation = ICombatInterface::Execute_GetAbilitySocketLocation(
		GetAvatarActorFromActorInfo(),
		GetAbilitySocketName(),
		IsUsingWeapon()
		);

  const FVector RelativeShapeOffset = GetAvatarActorFromActorInfo()->GetActorRotation().RotateVector(ShapeOffset);
  const FVector FinalLocation = AbilitySocketLocation + RelativeShapeOffset;

#if WITH_EDITOR
	if (bDebugAbility)
	{
		DrawDebugMeleeHitShape(FinalLocation);
	}
#endif

	const TArray<AActor*>& HitActors = ShapeScan(FinalLocation);

	for (const auto HitActor : HitActors)
	{
		ApplyEffectToHitActor(HitActor);
	  ActorsHitInMontage.Add(HitActor);
	}
}

void UMeleeAttackAbility::HandleWeaponCollision()
{
	UBoxComponent* WeaponBox = ICombatInterface::Execute_EnableWeaponCollision(GetAvatarActorFromActorInfo(), true);
	WeaponBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &UMeleeAttackAbility::OnWeaponCollision);
}

FComboData UMeleeAttackAbility::GetComboData() const
{
  FComboData ComboData = Super::GetComboData();

  if (!ComboData.bUseCustomData)
  {
    ComboData.MeleeHitMode = MeleeHitMode;
    ComboData.SphereHitRadius = SphereHitRadius;
    ComboData.BoxHitDimensions = BoxHitDimensions;
    ComboData.AngleWidth = AngleWidth;
    ComboData.AngleHeight = AngleHeight;
  }

  return ComboData;
}

EMeleeHitMode UMeleeAttackAbility::GetMeleeHitMode() const
{
  return bIsComboSequence ? GetComboData().MeleeHitMode : MeleeHitMode;
}

FVector UMeleeAttackAbility::GetShapeOffset() const
{
  return GetComboData().ShapeOffset;
}

FVector UMeleeAttackAbility::GetBoxHitDimensions() const
{
  return GetComboData().BoxHitDimensions;
}

float UMeleeAttackAbility::GetSphereHitRadius() const
{
  return GetComboData().SphereHitRadius;
}

float UMeleeAttackAbility::GetAngleWidth() const
{
  return GetComboData().AngleWidth;
}

float UMeleeAttackAbility::GetAngleHeight() const
{
  return GetComboData().AngleHeight;
}

#if WITH_EDITOR
void UMeleeAttackAbility::DrawDebugMeleeHitShape(const FVector& AbilitySocketLocation)
{
	switch (GetMeleeHitMode())
	{
	case EMeleeHitMode::Sphere:
		{
			DrawDebugSphere(
				GetWorld(),
				AbilitySocketLocation,
				GetSphereHitRadius(),
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
				GetBoxHitDimensions(),
				GetAvatarActorFromActorInfo()->GetActorQuat(),
				DrawShapeColor,
				false,
				DrawShapeDuration
			);
			break;
		}
	case EMeleeHitMode::Cone:
	  {
	    DrawDebugCone(
        GetWorld(),
        GetAvatarActorFromActorInfo()->GetActorLocation(),
        AbilitySocketLocation,
        FVector::Distance(AbilitySocketLocation, GetAvatarActorFromActorInfo()->GetActorLocation()),
        FMath::DegreesToRadians(GetAngleWidth()),
        FMath::DegreesToRadians(GetAngleHeight()),
        12,
        DrawShapeColor,
        false,
        DrawShapeDuration
      );
	    break;
	  }
	case EMeleeHitMode::LineTraces:
	  {
	    DrawDebugLine(
        GetWorld(),
        GetAvatarActorFromActorInfo()->GetActorLocation(),
        AbilitySocketLocation,
        DrawShapeColor,
        false,
        DrawShapeDuration,
        0,
        2.f
      );
	    break;
	  }
	case EMeleeHitMode::Weapon: {}
	default: {}
	}
}
#endif

TArray<AActor*> UMeleeAttackAbility::ShapeScan(const FVector& Origin)
{
	TArray ActorsToIgnore({ GetAvatarActorFromActorInfo() });
  ActorsToIgnore.Append(ActorsHitInMontage);
	TArray<AActor*> AliveActors;
	switch (GetMeleeHitMode())
	{
	case EMeleeHitMode::Sphere:
		{
			UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
				GetAvatarActorFromActorInfo(),
				AliveActors,
				ActorsToIgnore,
				GetSphereHitRadius(),
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
				GetBoxHitDimensions(),
				Origin,
				GetAvatarActorFromActorInfo()->GetActorQuat(),
				AbilityTargetTeam
			);
			break;
		}
	case EMeleeHitMode::Cone:
	case EMeleeHitMode::LineTraces:
	  {
	    UAuraAbilitySystemLibrary::GetAliveCharactersWithinLine(
	      GetAvatarActorFromActorInfo(),
	      AliveActors,
	      ActorsToIgnore,
	      GetAvatarActorFromActorInfo()->GetActorLocation(),
	      Origin,
	      AbilityTargetTeam
	      );
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

#if WITH_EDITOR
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
#endif
  
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
  if (ActorsHitInMontage.Contains(OtherActor)) return;

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
  ActorsHitInMontage.Add(OtherActor);
}
