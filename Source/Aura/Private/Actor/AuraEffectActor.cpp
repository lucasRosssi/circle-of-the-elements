// Copyright Lucas Rossi


#include "Actor/AuraEffectActor.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

