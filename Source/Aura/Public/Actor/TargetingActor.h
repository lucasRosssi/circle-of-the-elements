// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetingActor.generated.h"

UCLASS()
class AURA_API ATargetingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetingActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> TargetingDecal;
};
