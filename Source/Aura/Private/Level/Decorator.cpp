// Copyright Lucas Rossi


#include "Level/Decorator.h"

#include "Enums/Direction.h"
#include "Level/Tiles/Tile.h"

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
  
  Walls.Add(EDirection::North, Wall1);
  Walls.Add(EDirection::East, Wall2);
  Walls.Add(EDirection::South, Wall3);
  Walls.Add(EDirection::West, Wall4);
}

void ADecorator::RemoveWall(EDirection Direction)
{
  if (Walls.Contains(Direction))
  {
    Walls[Direction]->DestroyComponent();
  }
}

void ADecorator::BeginPlay()
{
	Super::BeginPlay();
	
}

ATile* ADecorator::GetParentTile()
{
  if (!ParentTile.IsValid())
  {
    ParentTile = Cast<ATile>(GetParentActor());
  }

  return ParentTile.Get();
}
