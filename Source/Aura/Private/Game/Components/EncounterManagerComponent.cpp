// Copyright Lucas Rossi


#include "Game/Components/EncounterManagerComponent.h"

#include "Actor/Level/EnemySpawner.h"
#include "Aura/AuraLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Level/RegionInfo.h"

UEncounterManagerComponent::UEncounterManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UEncounterManagerComponent::StartEncounter()
{
	EncountersCount += 1;
	
	NextWave();
}

void UEncounterManagerComponent::NextWave()
{
	if (EnemySpawners.Num() < EnemyWaves.Num())
	{
		UE_LOG(LogAura, Error, TEXT("Game Mode is trying to spawn more enemies this wave than there is available spawners"));
		return;
	}

	CurrentWave += 1;

	TArray<AEnemySpawner*> Spawners = EnemySpawners;
	for (const auto Enemy : EnemyWaves[0].Enemies)
	{
		const int32 SpawnerIndex = FMath::RandRange(0, Spawners.Num() - 1);
		if (Spawners.IsValidIndex(SpawnerIndex))
		{
			Spawners[SpawnerIndex]->AddEnemyClassToQueue(Enemy);
			Spawners[SpawnerIndex]->PrepareSpawn();
			Spawners.RemoveAt(SpawnerIndex);
		}
	}

	if (!bOverrideEnemyWaves) EnemyWaves.RemoveAt(0);
}

void UEncounterManagerComponent::FinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilationOnFinishEncounter);
	
	FTimerHandle SlowMotionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		SlowMotionTimerHandle,
		this,
		&UEncounterManagerComponent::PostFinishEncounter,
		TimeDilationResetDelay * UGameplayStatics::GetGlobalTimeDilation(this),
		false
		);
}

void UEncounterManagerComponent::PostFinishEncounter()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

	// for (const auto Player : Players)
	// {
	// 	IPlayerInterface::SafeExec_AddToXP(Player, StackedXP);
	// }
	// StackedXP = 0.f;

	// FActorSpawnParameters SpawnParameters;
	// FTransform Transform;
	// Transform.SetLocation(PlayerActor->GetActorLocation());
	//
	// const FRewardInfo& Info = RewardManager->GetNextRewardInfo();
	//
	// ALocationReward* Reward = GetWorld()->SpawnActorDeferred<ALocationReward>(
	// 	Info.RewardClass,
	// 	Transform,
	// 	nullptr,
	// 	nullptr,
	// 	ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	// 	);
	//
	// if (Reward)	Reward->FinishSpawning(Transform);
	// else UE_LOG(
	// 	LogAura,
	// 	Error,
	// 	TEXT("Failed to spawn Location Reward: %s"),
	// 	*Info.RewardClass->GetName()
	// 	);

	// OnEncounterFinishedDelegate.Broadcast();
}

void UEncounterManagerComponent::OnEnemySpawned(AActor* Enemy)
{
	EnemyCount += 1;
}

void UEncounterManagerComponent::OnEnemyKilled(AActor* Enemy)
{
	EnemyCount -= 1;
	if (EnemyCount <= 0)
	{
		if (CurrentWave == TotalWaves)
		{
			FinishEncounter();
		}
		else
		{
			if (WaveTransitionDelay > 0.f)
			{
				FTimerHandle WaveTransitionTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					WaveTransitionTimerHandle,
					this,
					&UEncounterManagerComponent::NextWave,
					WaveTransitionDelay,
					false
					);
			}
			else
			{
				NextWave();
			}
		}
	}
}

AAuraGameModeBase* UEncounterManagerComponent::GetAuraGameMode()
{
	if (AuraGameMode == nullptr)
	{
		UGameplayStatics::GetGameMode(GetOwner());
	}

	return AuraGameMode;
}
