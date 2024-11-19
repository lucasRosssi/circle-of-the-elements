// Copyright Lucas Rossi


#include "Level/Tiles/Tile.h"

#include "Components/TextRenderComponent.h"
#include "Enums/Direction.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;

  Root = CreateDefaultSubobject<USceneComponent>("Root");
  SetRootComponent(Root);

#if WITH_EDITOR
  TileNumberTextComponent = CreateDefaultSubobject<UTextRenderComponent>("TileNumberText");
  TileNumberTextComponent->SetupAttachment(Root);
  TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
  TileNumberTextComponent->SetVisibility(false);
#endif

  for (uint8 i = 0; i < static_cast<uint8>(EDirection::MAX); i++)
  {
    EDirection Direction = static_cast<EDirection>(i);
    AvailableExits.Add(Direction, false);
  }
}

void ATile::SetExitAvailable(EDirection WorldDirection, bool bAvailable)
{
  const EDirection RelativeDirection = GetRelativeDirection(WorldDirection);
  if (bool* Availability = AvailableExits.Find(RelativeDirection))
  {
    *Availability = bAvailable;
  }
  else
  {
    AvailableExits.Add(RelativeDirection, bAvailable);
  }
}

void ATile::SetTileNumber(int32 Number)
{
  TileNumber = Number;
  
#if WITH_EDITOR
  if (bDebug)
  {
    TileNumberTextComponent->SetVisibility(true);
    TileNumberTextComponent->SetText(FText::FormatOrdered(FText::FromString(FString("{0}")), TileNumber));
  }
#endif
}

void ATile::SetFacingDirection(EDirection Direction)
{
  FacingDirection = Direction;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
  
}

EDirection ATile::GetRelativeDirection(EDirection WorldDirection) const
{
  const int32 RelativeDirectionInt = static_cast<uint8>(FacingDirection);
  const int32 WorldDirectionInt = static_cast<uint8>(WorldDirection);
  int32 FinalDirectionInt = WorldDirectionInt - RelativeDirectionInt;

  if (FinalDirectionInt < 0)
  {
    FinalDirectionInt = 4 + FinalDirectionInt;
  }

  return static_cast<EDirection>(FinalDirectionInt);
}
