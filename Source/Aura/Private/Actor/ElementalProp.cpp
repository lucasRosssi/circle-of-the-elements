// Copyright Lucas Rossi


#include "Actor/ElementalProp.h"

#include "AuraGameplayTags.h"
#include "Aura/AuraMacros.h"

AElementalProp::AElementalProp()
{
	PrimaryActorTick.bCanEverTick = true;

  StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
  SetRootComponent(StaticMesh);
}

void AElementalProp::SetElementTag(const FGameplayTag& InElementTag)
{
  GUARD(InElementTag.MatchesTag(FAuraGameplayTags::Get().Abilities_Element),, TEXT("Tag must be an elemental tag! Tag provided: %s"), *InElementTag.ToString())

  ElementTag = InElementTag;
  OnElementSet();
}

void AElementalProp::BeginPlay()
{
	Super::BeginPlay();
	
}
