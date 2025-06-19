// Copyright Lucas Rossi


#include "Actor/SpiritActor.h"

ASpiritActor::ASpiritActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASpiritActor::SetAbilityTag(const FGameplayTag& InTag)
{
  AbilityTag = InTag;
}

void ASpiritActor::SetCooldownTag(const FGameplayTag& InTag)
{
  CooldownTag = InTag;
}

void ASpiritActor::BeginPlay()
{
	Super::BeginPlay();
	
}
