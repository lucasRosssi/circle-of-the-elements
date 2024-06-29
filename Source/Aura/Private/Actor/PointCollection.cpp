// Copyright Lucas Rossi


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	CentralPoint = CreateDefaultSubobject<USceneComponent>("CentralPoint");
	SetRootComponent(CentralPoint);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(
	const FVector& GroundLocation,
	float YawOverride
	)
{
	TArray<USceneComponent*> Points;
	GetRootComponent()->GetChildrenComponents(false, Points);

	TArray<USceneComponent*> ArrayCopy;
	for (const auto Pt : Points)
	{
		if (ArrayCopy.Num() >= GetRootComponent()->GetNumChildrenComponents())
		{
			FVector ToPoint = Pt->GetComponentLocation() - CentralPoint->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(CentralPoint->GetComponentLocation() + ToPoint);

			const FVector RaisedLocation = FVector(
				Pt->GetComponentLocation().X,
				Pt->GetComponentLocation().Y,
				Pt->GetComponentLocation().Z + 500.f
				);
			const FVector LoweredLocation = FVector(
				Pt->GetComponentLocation().X,
				Pt->GetComponentLocation().Y,
				Pt->GetComponentLocation().Z - 500.f
				);

			FHitResult HitResult;
			TArray<AActor*> IgnoreActors;
			TArray<AActor*> AliveIgnore;
			UAuraAbilitySystemLibrary::GetAliveCharactersWithinRadius(
				this,
				IgnoreActors,
				AliveIgnore,
				1500.f,
				GetActorLocation()
				);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActors(IgnoreActors);
			GetWorld()->LineTraceSingleByProfile(
				HitResult,
				RaisedLocation,
				LoweredLocation,
				FName("BlockAll"),
				QueryParams
			);

			const FVector AdjustedLocation = FVector(
					Pt->GetComponentLocation().X,
					Pt->GetComponentLocation().Y,
					HitResult.ImpactPoint.Z
				);
			Pt->SetWorldLocation(AdjustedLocation);
			Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

			ArrayCopy.Add(Pt);
		}
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
