// Copyright Lucas Rossi


#include "AbilitySystem/Abilities/SummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FVector Forward = AvatarActor->GetActorForwardVector();
	const FVector Location = AvatarActor->GetActorLocation();
	const float DeltaSpread = SpawnSpread / MinionsPerSummon;

	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2, FVector::UpVector);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < MinionsPerSummon; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const FVector ChosenSpawnLocation = Location + Direction *
			FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(ChosenSpawnLocation);
		DrawDebugSphere(
			GetWorld(),
			ChosenSpawnLocation,
			20.f,
			12,
			FColor::Yellow,
			false,
			3.f
		);
		UKismetSystemLibrary::DrawDebugArrow(
			AvatarActor,
			Location + Direction * MinSpawnDistance,
			Location + Direction * MaxSpawnDistance,
			40.f,
			FLinearColor::Green,
			3.f
		);
	}

	return SpawnLocations;
}
