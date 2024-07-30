// Copyright Lucas Rossi


#include "Actor/Level/EnemySpawner.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/AuraEnemy.h"
#include "Components/CapsuleComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemySpawner::AddEnemyClassToQueue(TSubclassOf<AAuraEnemy> EnemyClass)
{
	EnemyQueue.Add(EnemyClass);
}

void AEnemySpawner::PreSpawn()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		SpawnEffect,
		GetActorLocation(),
		GetActorRotation()
		);

	if (SpawnDelay > 0.f)
	{
		FTimerHandle SpawnTimer;
		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			this,
			&AEnemySpawner::SpawnNextEnemy,
			SpawnDelay,
			false
		);
	}
	else
	{
		SpawnNextEnemy();
	}
}

void AEnemySpawner::SpawnNextEnemy()
{
	if (EnemyQueue.IsEmpty()) return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation() + FVector(0.f, 0.f, 400.f),
		GetActorLocation() - FVector(0.f, 0.f, 400.f),
		ECC_Visibility
	);
	
	if (HitResult.bBlockingHit)
	{
		SpawnTransform.SetLocation(HitResult.ImpactPoint);
	}
	else
	{
		SpawnTransform.SetLocation(GetActorLocation());
	}
	
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(
		EnemyQueue[0],
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

	SpawnTransform.SetLocation(FVector(
		SpawnTransform.GetLocation().X,
		SpawnTransform.GetLocation().Y,
		SpawnTransform.GetLocation().Z + Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()
		));
	Enemy->SetLevel(EnemyLevel);
	Enemy->GetOnDeathDelegate().AddDynamic(this, &AEnemySpawner::OnSpawnedEnemyDeath);
	Enemy->FinishSpawning(SpawnTransform);
	EnemySpawnedDelegate.Broadcast(Enemy);

	EnemyQueue.RemoveAt(0);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawner::OnSpawnedEnemyDeath(AActor* Enemy)
{
	SpawnedEnemyDeathDelegate.Broadcast(Enemy);
}
