// Copyright Lucas Rossi


#include "Level/Tile.h"

#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"

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

  TileNumberTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TileNumberText");
  TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
}

void ATile::BeginPlay()
{
	Super::BeginPlay();

  
}
