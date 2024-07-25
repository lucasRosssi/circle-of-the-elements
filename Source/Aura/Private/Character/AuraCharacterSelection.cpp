// Copyright Lucas Rossi


#include "Character/AuraCharacterSelection.h"

#include "Aura/Aura.h"

AAuraCharacterSelection::AAuraCharacterSelection()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
}

void AAuraCharacterSelection::HighlightActor_Implementation(AActor* InstigatorActor)
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
}

void AAuraCharacterSelection::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
