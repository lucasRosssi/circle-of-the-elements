// Copyright Lucas Rossi


#include "Components/InteractComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/InteractionAbility.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/InteractInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

UInteractComponent::UInteractComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
  
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
  if (!bInteractionEnabled || !IsValid(OtherActor) || !OtherActor->Implements<UPlayerInterface>()) return;

  switch (InteractMode)
  {
  case EInteractMode::Overlap:
    {
      BeginInteract(OtherActor->GetInstigatorController());
      break;
    }
  case EInteractMode::Input:
  default:
    {
      IPlayerInterface::Safe_AddInteractableToList(OtherActor, this);
    }
  }
  
}

void UInteractComponent::OnInteractAreaEndOverlap(
  UPrimitiveComponent* OverlappedComponent,
  AActor* OtherActor,
  UPrimitiveComponent* OtherComp,
  int32 OtherBodyIndex
)
{
  if (!bInteractionEnabled || !IsValid(OtherActor)) return;

  switch (InteractMode)
  {
  case EInteractMode::Overlap:
    {
      break;
    }
  case EInteractMode::Input:
  default:
    {
      IPlayerInterface::Safe_RemoveInteractableFromList(OtherActor, this);
    }
  }
  
}

void UInteractComponent::SetInteractAreaComponent(UPrimitiveComponent* InComponent)
{
  InteractAreaComponent = InComponent;
  InteractAreaComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
  InteractAreaComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  InteractAreaComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  InteractAreaComponent->SetUsingAbsoluteScale(true);
}

void UInteractComponent::EnableInteraction()
{
  bInteractionEnabled = true;
  InteractAreaComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UInteractComponent::DisableInteraction()
{
  bInteractionEnabled = false;
  InteractAreaComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UInteractComponent::BeginInteract(const AController* InstigatorController)
{
  ACharacter* Character = InstigatorController->GetCharacter();
  if (bDisableAfterInteraction)
  {
    DisableInteraction();
    IPlayerInterface::Safe_RemoveInteractableFromList(Character, this);
  }

  if (Character->Implements<UCombatInterface>())
  {
    ICombatInterface::Execute_UpdateFacingTarget(Character, GetOwner()->GetActorLocation());
  }
  
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
  
  UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
  InstigatorController->GetPawn()
    );
  if (ASC)
  {
    if (InteractEffect)
    {
      FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
      ContextHandle.AddSourceObject(this);
      const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
        InteractEffect,
       1, 
      ContextHandle);
      ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
    }

    if (InteractAbility)
    {
      FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InteractAbility, 1);
      FGameplayEventData Data = IInteractInterface::Safe_GetAbilityEventData(GetOwner());
      if (!Data.Instigator) Data.Instigator = InstigatorController->GetPawn();
      if (!Data.Target) Data.Target = GetOwner();
      
      ASC->GiveAbilityAndActivateOnce(AbilitySpec, &Data);
    }
  }

  IInteractInterface::Safe_Interact(GetOwner(), InstigatorController);
}

void UInteractComponent::BeginPlay()
{
  Super::BeginPlay();

  InteractAreaComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaOverlap);
  InteractAreaComponent->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OnInteractAreaEndOverlap);

  if (!bInteractionEnabled)
  {
    DisableInteraction();
  }

#if WITH_EDITOR
  if (!GetOwner()->Implements<UInteractInterface>())
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT(
        "%s has an InteractComponent, but doesn't implement InteractInterface. Interaction may fail,"
        "please add the interface to %s class."
      ),
      *GetOwner()->GetName(),
      *GetOwner()->GetClass()->GetName()
    )
  }
#endif
}
