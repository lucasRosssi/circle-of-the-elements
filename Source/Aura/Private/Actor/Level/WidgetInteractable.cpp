// Copyright Lucas Rossi


#include "Actor/Level/WidgetInteractable.h"

#include "Components/InteractComponent.h"
#include "Managers/UIManager.h"
#include "Utils/AuraSystemsLibrary.h"

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

void AWidgetInteractable::BeginPlay()
{
	Super::BeginPlay();

  InteractComponent->OnInteractedDelegate.BindUObject(this, &AWidgetInteractable::Interact);
}

void AWidgetInteractable::Interact(const AAuraPlayerController* Controller)
{
  UAuraSystemsLibrary::GetUIManager(this, 0)->BuildAndOpenWidget(WidgetClass);
}
