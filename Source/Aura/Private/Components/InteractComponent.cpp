// Copyright Lucas Rossi


#include "Components/InteractComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

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
  
  IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
  if (!PlayerInterface) return;

  PlayerInterface->GetOnInteractDelegate().AddUniqueDynamic(this, &UInteractComponent::PreInteract);
  IPlayerInterface::Execute_SetInteractMessageVisible(OtherActor, true);
}

void UInteractComponent::OnInteractAreaEndOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex
)
{
  if (!IsValid(OtherActor) || OtherActor->IsActorBeingDestroyed()) return;
  if (IsBeingDestroyed()) return;
  
  IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
  if (!PlayerInterface) return;

  FOnInteract& OnInteractDelegate = PlayerInterface->GetOnInteractDelegate();
  if (OnInteractDelegate.IsBound())
  {
    OnInteractDelegate.RemoveAll(this);
  }
  
  IPlayerInterface::Execute_SetInteractMessageVisible(OtherActor, false);
}

float UInteractComponent::GetInteractAreaRadius()
{
  return InteractArea->GetScaledSphereRadius();
}

void UInteractComponent::PreInteract(const AAuraPlayerController* InstigatorController)
{
  if (!bInteractionEnabled) return;

  IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(InstigatorController->GetPawn());
  if (bDisableAfterInteraction && PlayerInterface)
  {
    PlayerInterface->GetOnInteractDelegate().RemoveAll(this);
    IPlayerInterface::Execute_SetInteractMessageVisible(InstigatorController->GetPawn(), false);
  }

  if (InstigatorController->GetPawn()->Implements<UCombatInterface>())
  {
    ICombatInterface::Execute_UpdateFacingTarget(InstigatorController->GetPawn(), GetOwner()->GetActorLocation());
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

  Interact(InstigatorController);
  PostInteract(InstigatorController);
}

void UInteractComponent::Interact(const AAuraPlayerController* InstigatorController)
{
  const bool bInteractionSuccessful = OnInteractedDelegate.ExecuteIfBound(InstigatorController);

  if (!bInteractionSuccessful)
  {
    UE_LOG(LogAura, Error, TEXT("Interaction of %s failed!"), *GetOwner()->GetName())
  }
}

void UInteractComponent::PostInteract(const AAuraPlayerController* InstigatorController)
{
  if (bDisableAfterInteraction)
  {
    DisableInteraction();
  }

  // ReSharper disable once CppExpressionWithoutSideEffects
  OnPostInteractedDelegate.ExecuteIfBound(InstigatorController);
}

void UInteractComponent::BeginPlay()
{
  Super::BeginPlay();

  InteractArea->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaOverlap);
  InteractArea->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaEndOverlap);
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
