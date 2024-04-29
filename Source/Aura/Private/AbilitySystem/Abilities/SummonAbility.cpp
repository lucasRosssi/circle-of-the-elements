// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/SummonAbility.h"

#include "Character/AuraCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FVector Forward = AvatarActor->GetActorForwardVector();
	const FVector Location = AvatarActor->GetActorLocation();
	const float DeltaSpread = SpawnSpread / MinionsPerSummon;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < FMath::Min(MinionsPerSummon, MaxMinions - ActiveMinions); i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		
		FVector ChosenSpawnLocation = Location + Direction *
			FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility
		);

		if (HitResult.bBlockingHit)
		{
			ChosenSpawnLocation = HitResult.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<AAuraCharacterBase> USummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);

	return MinionClasses[Selection];
}

AAuraCharacterBase* USummonAbility::SpawnMinion(FVector Location)
{
	if (HasMaxMinionsActive())
	{
		return nullptr;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const TSubclassOf<AAuraCharacterBase> MinionClass = GetRandomMinionClass();
	const AAuraCharacterBase* MinionDefaultObject =
		MinionClass->GetDefaultObject<AAuraCharacterBase>();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Cast<APawn>(AvatarActor);
	SpawnParameters.Owner = AvatarActor;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
	AAuraCharacterBase* Minion = GetWorld()->SpawnActor<AAuraCharacterBase>(
		MinionClass,
		Location + FVector(
			0.f,
			0.f,
			MinionDefaultObject->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
			),
		UKismetMathLibrary::FindLookAtRotation(AvatarActor->GetActorLocation(), Location),
		SpawnParameters
	);

	if (Minion)
	{
		Minion->OnSummon();
		ActiveMinions++;
	}
	
	return Minion;
}

bool USummonAbility::HasMaxMinionsActive()
{
	return ActiveMinions >= MaxMinions;
}
