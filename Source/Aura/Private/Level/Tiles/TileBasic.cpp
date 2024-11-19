// Copyright Lucas Rossi


#include "Level/Tiles/TileBasic.h"

#include "Components/ArrowComponent.h"
#include "Enums/Direction.h"

ATileBasic::ATileBasic()
{
  UnderWallMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh1");
  UnderWallMesh1->SetupAttachment(TileMesh);
  UnderWallMesh2 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh2");
  UnderWallMesh2->SetupAttachment(TileMesh);
  UnderWallMesh3 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh3");
  UnderWallMesh3->SetupAttachment(TileMesh);
  UnderWallMesh4 = CreateDefaultSubobject<UStaticMeshComponent>("UnderWallMesh4");
  UnderWallMesh4->SetupAttachment(TileMesh);
  
#if WITH_EDITOR
  NorthArrow = CreateDefaultSubobject<UArrowComponent>("NorthArrow");
  NorthArrow->SetupAttachment(TileMesh);
  EastArrow = CreateDefaultSubobject<UArrowComponent>("EastArrow");
  EastArrow->SetupAttachment(TileMesh);
  EastArrow->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
  SouthArrow = CreateDefaultSubobject<UArrowComponent>("SouthArrow");
  SouthArrow->SetupAttachment(TileMesh);
  SouthArrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
  WestArrow = CreateDefaultSubobject<UArrowComponent>("WestArrow");
  WestArrow->SetupAttachment(TileMesh);
  WestArrow->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));

  Arrows.Add(EDirection::North, NorthArrow);
  Arrows.Add(EDirection::East, EastArrow);
  Arrows.Add(EDirection::South, SouthArrow);
  Arrows.Add(EDirection::West, WestArrow);
#endif
}

void ATileBasic::SetExitAvailable(EDirection Direction, bool bAvailable)
{
  Super::SetExitAvailable(Direction, bAvailable);

#if WITH_EDITOR
  if (bDebug && Arrows.Contains(Direction))
  {
    Arrows[Direction]->SetVisibility(bAvailable);
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
