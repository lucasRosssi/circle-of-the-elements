// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"

#define NEUTRAL_TEAM 0
#define PLAYER_TEAM 1
#define GOBLIN_TEAM 2

#define HOSTILE_TO_ALL -1

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_GREEN 251
#define CUSTOM_DEPTH_YELLOW 252

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludeCharacters ECollisionChannel::ECC_GameTraceChannel3

#define ETT_SingleHitTrace ETraceTypeQuery::TraceTypeQuery3
#define ETT_MultiHitTrace ETraceTypeQuery::TraceTypeQuery4
#define ETT_CameraToPlayer ETraceTypeQuery::TraceTypeQuery5
