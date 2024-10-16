// Copyright Lucas Rossi


#include "Character/BossEnemy.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/AuraLogChannels.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Managers/EncounterManager.h"
#include "Utils/AuraSystemsLibrary.h"

ABossEnemy::ABossEnemy()
{
	CharacterType = ECharacterType::Boss;

	HealthBar->Deactivate();
	HealthBar->SetVisibility(false);
}

void ABossEnemy::NextBossState()
{
	if (!BossStateEffects.IsValidIndex(BossState))
	{
		UE_LOG(LogAura, Error, TEXT("Failed to change boss state. GameplayEffects not set in BossStateEffects"));
		return;
	}

	ApplyEffectToSelf(BossStateEffects[BossState], 1.f);
	BossState += 1;
	AuraAIController->GetBlackboardComponent()->SetValueAsInt(FName("BossState"), BossState);

	if (!BossStateHealthThresholds.IsValidIndex(BossState))
	{
		OnHealthChanged.RemoveDynamic(this, &ABossEnemy::CheckHealth);
	}
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!BossStateHealthThresholds.IsEmpty()) OnHealthChanged.AddDynamic(this, &ABossEnemy::CheckHealth);
  
  UAuraSystemsLibrary::GetEncounterManager(this)->SetCurrentBoss(this);
}

void ABossEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AuraAIController->GetBlackboardComponent()->SetValueAsInt(FName("BossState"), BossState);
}

void ABossEnemy::CheckHealth(float NewHealth)
{
	const float MaxHealth = AttributeSet->GetMaxHealth();
	if (NewHealth / MaxHealth <= BossStateHealthThresholds[BossState])
	{
		NextBossState();
	}
}
