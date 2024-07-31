// Copyright Lucas Rossi


#include "Actor/Level/EnemySpawner.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/AuraEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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

void AEnemySpawner::PrepareSpawn()
{
	if (EnemyQueue.IsEmpty()) return;

	if (PreSpawnDelayMax > 0.f)
	{
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(
			DelayHandle,
			this,
			&AEnemySpawner::PreSpawn,
			FMath::RandRange(0.f, PreSpawnDelayMax),
			false
			);
	}
	else
	{
		PreSpawn();
	}
}

void AEnemySpawner::SpawnNextEnemy()
{
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

	if (bOverrideEnemyLevel)
	{
		EnemyLevel = LevelOverride;
	}
	else
	{
		const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(
			UGameplayStatics::GetGameMode(this)
			);
		
		EnemyLevel = AuraGM->GetEnemiesLevel();
	}
}

void AEnemySpawner::OnSpawnedEnemyDeath(AActor* Enemy)
{
	SpawnedEnemyDeathDelegate.Broadcast(Enemy);
}
