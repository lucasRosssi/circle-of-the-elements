// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileEffect.generated.h"

class UNiagaraComponent;

UCLASS()
class AURA_API AProjectileEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileEffect();

	void DeactivateNiagara();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float LifeSpanAfterDeactivation = 1.f;
	
};
