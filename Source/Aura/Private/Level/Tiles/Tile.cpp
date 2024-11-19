// Copyright Lucas Rossi


#include "Level/Tiles/Tile.h"

#include "Components/TextRenderComponent.h"
#include "Enums/Direction.h"
#include "Level/Decorator.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

  TileMesh = CreateDefaultSubobject<UStaticMeshComponent>("TileMesh");
  SetRootComponent(TileMesh);

#if WITH_EDITOR
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

  if (GetDecorator())
  {
    Decorator->RemoveWall(Direction);
  }
}

void ATile::SetTileNumber(int32 Number)
{
  TileNumber = Number;
  
#if WITH_EDITOR
  TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
#endif
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
  
}

AActor* ATile::GetDecorator()
{
  if (Decorator == nullptr)
  {
    Decorator = Cast<ADecorator>(ChildDecoratorComponent->GetChildActor());
  }

  return Decorator;
}
