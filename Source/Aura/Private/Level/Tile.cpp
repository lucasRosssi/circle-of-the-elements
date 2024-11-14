// Copyright Lucas Rossi


#include "Level/Tile.h"

#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Enums/Direction.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

  TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
  SetRootComponent(TileMesh);
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

  TileNumberTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TileNumberText");
  TileNumberTextComponent->SetupAttachment(TileMesh);
  TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
#endif

  for (uint8 i = 0; i < static_cast<uint8>(EDirection::MAX); i++)
  {
    EDirection Direction = static_cast<EDirection>(i);
    AvailableExits.Add(Direction, false);
  }

  ChildDecoratorComponent = CreateDefaultSubobject<UChildActorComponent>("Decorator");
  ChildDecoratorComponent->SetupAttachment(TileMesh);
}

void ATile::SetExitAvailable(EDirection Direction, bool bAvailable)
{
  if (bool* Availability = AvailableExits.Find(Direction))
  {
    *Availability = bAvailable;
  }
  else
  {
    AvailableExits.Add(Direction, bAvailable);
  }

  if (Arrows.Contains(Direction))
  {
    Arrows[Direction]->SetVisibility(bAvailable);
  }
}

void ATile::SetTileNumber(int32 Number)
{
  TileNumber = Number;
  TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

  for (auto [Direction, bAvailable] : AvailableExits)
  {
    if (Arrows.Contains(Direction))
    {
      Arrows[Direction]->SetVisibility(bAvailable);
    }
  }
}

AActor* ATile::GetDecorator()
{
  if (Decorator == nullptr)
  {
    Decorator = Cast<AActor>(ChildDecoratorComponent->GetChildActor());
  }

  return Decorator;
}
