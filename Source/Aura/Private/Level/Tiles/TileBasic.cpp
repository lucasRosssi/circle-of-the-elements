// Copyright Lucas Rossi


#include "Level/Tiles/TileBasic.h"

#include "Components/ArrowComponent.h"
#include "Enums/Direction.h"
#include "Level/Decorator.h"

ATileBasic::ATileBasic()
{
  FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
  FloorMesh->SetupAttachment(GetRootComponent());

  UnderWallMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh1");
  UnderWallMesh1->SetupAttachment(GetRootComponent());
  UnderWallMesh2 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh2");
  UnderWallMesh2->SetupAttachment(GetRootComponent());
  UnderWallMesh3 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh3");
  UnderWallMesh3->SetupAttachment(GetRootComponent());
  UnderWallMesh4 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh4");
  UnderWallMesh4->SetupAttachment(GetRootComponent());

  ChildDecoratorComponent = CreateDefaultSubobject<UChildActorComponent>("Decorator");
  ChildDecoratorComponent->SetupAttachment(GetRootComponent());
  
#if WITH_EDITOR
  NorthArrow = CreateDefaultSubobject<UArrowComponent>("NorthArrow");
  NorthArrow->SetupAttachment(GetRootComponent());
  EastArrow = CreateDefaultSubobject<UArrowComponent>("EastArrow");
  EastArrow->SetupAttachment(GetRootComponent());
  EastArrow->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
  SouthArrow = CreateDefaultSubobject<UArrowComponent>("SouthArrow");
  SouthArrow->SetupAttachment(GetRootComponent());
  SouthArrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
  WestArrow = CreateDefaultSubobject<UArrowComponent>("WestArrow");
  WestArrow->SetupAttachment(GetRootComponent());
  WestArrow->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

  Arrows.Add(EDirection::North, NorthArrow);
  Arrows.Add(EDirection::East, EastArrow);
  Arrows.Add(EDirection::South, SouthArrow);
  Arrows.Add(EDirection::West, WestArrow);
#endif
}

void ATileBasic::SetExitAvailable(EDirection WorldDirection, bool bAvailable)
{
  Super::SetExitAvailable(WorldDirection, bAvailable);

  const EDirection RelativeDirection = GetRelativeDirection(WorldDirection);
  
  if (GetDecorator())
  {
    Decorator->RemoveWall(RelativeDirection);
  }

#if WITH_EDITOR
  if (bDebug && Arrows.Contains(RelativeDirection))
  {
    Arrows[RelativeDirection]->SetVisibility(bAvailable);
  }
#endif
}

void ATileBasic::BeginPlay()
{
  Super::BeginPlay();

#if WITH_EDITOR
  for (auto [Direction, bAvailable] : AvailableExits)
  {
    if (bDebug && Arrows.Contains(Direction))
    {
      Arrows[Direction]->SetVisibility(bAvailable);
    }
  }
#endif
}

ADecorator* ATileBasic::GetDecorator()
{
  if (Decorator == nullptr)
  {
    Decorator = Cast<ADecorator>(ChildDecoratorComponent->GetChildActor());
  }

  return Decorator;
}
