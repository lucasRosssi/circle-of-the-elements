// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"

#define HOSTILE_TO_ALL -1
#define NEUTRAL_TEAM 0
#define PLAYER_TEAM 1
#define GOBLIN_TEAM 2

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_GREEN 251
#define CUSTOM_DEPTH_WHITE 252

#define CUSTOM_DEPTH_FULL_RED 255
#define CUSTOM_DEPTH_DEFAULT_COLOR 254

#define MAX_SAVE_SLOTS 3
#define SLOT_DEFAULT_NAME FString("SaveSlot_")

#define MAX_LEVEL 20

#define DIRECTION_CLOSED_PROP_TAG FName("DirClosed")
#define DIRECTION_OPEN_PROP_TAG FName("DirOpen")

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludeCharacters ECollisionChannel::ECC_GameTraceChannel5

#define ETT_SingleHitTrace ETraceTypeQuery::TraceTypeQuery3
#define ETT_MultiHitTrace ETraceTypeQuery::TraceTypeQuery4
#define ETT_CameraToPlayer ETraceTypeQuery::TraceTypeQuery5
#define ETT_AimAssist ETraceTypeQuery::TraceTypeQuery6
