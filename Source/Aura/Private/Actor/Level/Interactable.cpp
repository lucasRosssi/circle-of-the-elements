// Copyright Lucas Rossi


#include "Actor/Level/Interactable.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractArea = CreateDefaultSubobject<USphereComponent>("InteractArea");
	SetRootComponent(InteractArea);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

float AInteractable::GetInteractAreaRadius()
{
	return InteractArea->GetScaledSphereRadius();
}

void AInteractable::PreInteract(AAuraPlayerController* InstigatorController)
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

	DisableInteraction();
}

void AInteractable::Interact(AAuraPlayerController* InstigatorController)
{
	if (InstigatorController->GetPawn()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_UpdateFacingTarget(InstigatorController->GetPawn(), GetActorLocation());
	}
	
	if (InteractMontages.Num() > 0 && InstigatorController->GetCharacter()->Implements<UPlayerInterface>())
	{
		const ECharacterName HeroName = IPlayerInterface::Execute_GetHeroName(InstigatorController->GetCharacter());
		if (const auto Montage = InteractMontages.Find(HeroName))
		{
			InstigatorController->GetCharacter()->PlayAnimMontage(*Montage);
		}
	}

	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			InteractSound,
			GetActorLocation(),
			GetActorRotation()
			);
	}
	if (InteractNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			InteractNiagaraSystem,
			GetActorLocation() + InteractNiagaraOffset
			);
	}
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	InteractArea->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnInteractAreaOverlap);
	InteractArea->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnInteractAreaEndOverlap);

	if (SpawnNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			SpawnNiagaraSystem,
			GetActorLocation() + SpawnNiagaraOffset
			);
	}
	if (IdleNiagaraSystem)
	{
		IdleNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			IdleNiagaraSystem,
			GetActorLocation() + IdleNiagaraOffset
			);
	}
}

void AInteractable::Destroyed()
{
	if (IdleNiagaraComponent)
	{
		IdleNiagaraComponent->Deactivate();
	}
	
	Super::Destroyed();
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

	return AuraGameMode.Get();
}
