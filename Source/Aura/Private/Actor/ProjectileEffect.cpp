// Copyright Lucas Rossi


#include "Actor/ProjectileEffect.h"

#include "NiagaraComponent.h"

AProjectileEffect::AProjectileEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("Niagara Component"));
	SetRootComponent(NiagaraComponent);
	SetActorEnableCollision(false);
}

void AProjectileEffect::DeactivateNiagara()
{
	NiagaraComponent->Deactivate();
	SetLifeSpan(LifeSpanAfterDeactivation);
}

// Called when the game starts or when spawned
void AProjectileEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

