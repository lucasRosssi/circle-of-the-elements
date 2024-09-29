// Copyright Lucas Rossi


#include "Actor/Level/WidgetInteractable.h"

#include "Components/InteractComponent.h"
#include "Managers/UIManager.h"
#include "Player/AuraPlayerController.h"

AWidgetInteractable::AWidgetInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

  StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
  SetRootComponent(StaticMesh);

  InteractComponent = CreateDefaultSubobject<UInteractComponent>("InteractComponent");
  InteractComponent->SetupInteractAreaAttachment(GetRootComponent());
  InteractComponent->EnableInteraction();
  InteractComponent->bDisableAfterInteraction = false;

}

void AWidgetInteractable::Interact_Implementation(const AController* Controller)
{
  const AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(Controller);
  if (!AuraPC) return;
  
  AuraPC->GetUIManager()->BuildAndOpenWidget(WidgetClass);
}

UInteractComponent* AWidgetInteractable::GetInteractComponent_Implementation() const
{
  return InteractComponent;
}

void AWidgetInteractable::BeginPlay()
{
	Super::BeginPlay();

}
