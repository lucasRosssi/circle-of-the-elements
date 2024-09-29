// Copyright Lucas Rossi


#include "Components/InteractComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

UInteractComponent::UInteractComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
  
  InteractArea = CreateDefaultSubobject<USphereComponent>("InteractArea");
  InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
  InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  InteractArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  InteractArea->SetSphereRadius(200.f, false);
  InteractArea->SetUsingAbsoluteScale(true);
}

void UInteractComponent::SetupInteractAreaAttachment(USceneComponent* Component)
{
  InteractArea->SetupAttachment(Component);
}

void UInteractComponent::OnInteractAreaOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex,
  bool bFromSweep,
  const FHitResult& SweepResult
)
{
  if (!IsValid(OtherActor)) return;
  
  IPlayerInterface::Safe_AddInteractableToList(OtherActor, this);
}

void UInteractComponent::OnInteractAreaEndOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex
)
{
  if (!IsValid(OtherActor)) return;
  
  IPlayerInterface::Safe_RemoveInteractableFromList(OtherActor, this);
}

float UInteractComponent::GetInteractAreaRadius() const
{
  return InteractArea->GetScaledSphereRadius();
}

void UInteractComponent::EnableInteraction()
{
  bInteractionEnabled = true;
  InteractArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UInteractComponent::DisableInteraction()
{
  bInteractionEnabled = false;
  InteractArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UInteractComponent::BeginInteract(const AController* InstigatorController) const
{
  ACharacter* Character = InstigatorController->GetCharacter();
  if (bDisableAfterInteraction)
  {
    IPlayerInterface::Safe_RemoveInteractableFromList(Character, this);
  }

  // if (Character->Implements<UCombatInterface>())
  // {
  //   ICombatInterface::Execute_UpdateFacingTarget(Character, GetOwner()->GetActorLocation());
  // }
  
  if (InteractMontages.Num() > 0 && Character->Implements<UPlayerInterface>())
  {
    const ECharacterName HeroName = IPlayerInterface::Execute_GetHeroName(Character);
    if (const auto Montage = InteractMontages.Find(HeroName))
    {
      Character->PlayAnimMontage(*Montage);
    }
  }

  if (InteractSound)
  {
    UGameplayStatics::PlaySoundAtLocation(
      this,
      InteractSound,
      GetOwner()->GetActorLocation(),
      GetOwner()->GetActorRotation()
    );
  }
  if (InteractNiagaraSystem)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
      this,
      InteractNiagaraSystem,
      GetOwner()->GetActorLocation() + InteractNiagaraOffset
    );
  }

  IInteractInterface::Safe_Interact(GetOwner(), InstigatorController);
}

void UInteractComponent::BeginPlay()
{
  Super::BeginPlay();

  InteractArea->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaOverlap);
  InteractArea->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaEndOverlap);
}
