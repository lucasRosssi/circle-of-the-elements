// Copyright Lucas Rossi


#include "Level/Tiles/Special/TileExit.h"

ATileExit::ATileExit()
{
  ChildGateComponent = CreateDefaultSubobject<UChildActorComponent>("GateComponent");
  ChildGateComponent->SetupAttachment(GetRootComponent());
}
