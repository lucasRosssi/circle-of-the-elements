// Copyright Lucas Rossi


#include "Actor/Level/Interactable.h"

#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractArea = CreateDefaultSubobject<USphereComponent>("InteractArea");
	SetRootComponent(InteractArea);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AInteractable::OnInteractAreaOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
	if (!PlayerInterface) return;

	PlayerInterface->GetOnInteractDelegate().AddDynamic(this, &AInteractable::OnInteracted);
}

void AInteractable::OnInteractAreaEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
	)
{
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
	if (!PlayerInterface) return;

	PlayerInterface->GetOnInteractDelegate().RemoveDynamic(this, &AInteractable::OnInteracted);
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	InteractArea->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnInteractAreaOverlap);
	InteractArea->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnInteractAreaEndOverlap);
}
