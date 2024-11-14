// Copyright Lucas Rossi


#include "Level/Decorator.h"

ADecorator::ADecorator()
{
	PrimaryActorTick.bCanEverTick = true;

  Root = CreateDefaultSubobject<USceneComponent>("Root");
  SetRootComponent(Root);
  Wall1 = CreateDefaultSubobject<UStaticMeshComponent>("Wall1");
  Wall1->SetupAttachment(Root);
  Wall2 = CreateDefaultSubobject<UStaticMeshComponent>("Wall2");
  Wall2->SetupAttachment(Root);
  Wall3 = CreateDefaultSubobject<UStaticMeshComponent>("Wall3");
  Wall3->SetupAttachment(Root);
  Wall4 = CreateDefaultSubobject<UStaticMeshComponent>("Wall4");
  Wall4->SetupAttachment(Root);
}

void ADecorator::BeginPlay()
{
	Super::BeginPlay();
	
}
