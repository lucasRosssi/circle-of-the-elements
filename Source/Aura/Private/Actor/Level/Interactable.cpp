// Copyright Lucas Rossi


#include "Actor/Level/Interactable.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractArea = CreateDefaultSubobject<USphereComponent>("InteractArea");
	SetRootComponent(InteractArea);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AInteractable::PreInteract(AController* InstigatorController)
{
	if (!bInteractionEnabled) return;

	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(InstigatorController->GetPawn());
	if (PlayerInterface)
	{
		PlayerInterface->GetOnInteractDelegate().RemoveAll(this);
		IPlayerInterface::Execute_SetInteractMessageVisible(InstigatorController->GetPawn(), false);
	}
	
	Interact(InstigatorController);
	OnInteracted(InstigatorController);
}

void AInteractable::Interact(AController* InstigatorController)
{
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

	PlayerInterface->GetOnInteractDelegate().AddUniqueDynamic(this, &AInteractable::PreInteract);
	IPlayerInterface::Execute_SetInteractMessageVisible(OtherActor, true);
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

	PlayerInterface->GetOnInteractDelegate().RemoveAll(this);
	IPlayerInterface::Execute_SetInteractMessageVisible(OtherActor, false);
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	InteractArea->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnInteractAreaOverlap);
	InteractArea->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnInteractAreaEndOverlap);
}

void AInteractable::EnableInteraction()
{
	bInteractionEnabled = true;
	InteractArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractable::DisableInteraction()
{
	bInteractionEnabled = false;
	InteractArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

AAuraGameModeBase* AInteractable::GetAuraGameMode()
{
	if (AuraGameMode == nullptr)
	{
		AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	}

	return AuraGameMode;
}
