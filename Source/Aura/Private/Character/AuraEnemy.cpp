// Copyright Lucas Rossi


#include "Character/AuraEnemy.h"

#include "NavigationInvokerComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Components/WidgetComponent.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/TeamComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>("NavigationInvoker");
	NavigationInvoker->SetGenerationRadii(1500.f, 1500.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>
	("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated((true));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	TeamComponent->TeamID = GOBLIN_TEAM;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);

	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::HitReactTagChanged(CallbackTag, NewCount);

	if (!HasAuthority()) return;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

int32 AAuraEnemy::GetCharacterLevel_Implementation() const
{
	return Level;
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpanDuration);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	
	Super::Die(DeathImpulse);
}

void AAuraEnemy::SetActionSpeed_Implementation(float InActionSpeed)
{
	ChangeActionSpeed(InActionSpeed);
}

float AAuraEnemy::GetPower_Implementation()
{
	return AttributeSet->GetPower();
}

float AAuraEnemy::GetMaxHealth_Implementation()
{
	return AttributeSet->GetMaxHealth();
}

float AAuraEnemy::GetHealth_Implementation()
{
	return AttributeSet->GetHealth();
}

void AAuraEnemy::SetTimeDilation_Implementation(float InTimeDilation)
{
	CustomTimeDilation = InTimeDilation;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	if (HasAuthority())
	{
		AddCharacterAbilities();
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (AttributeSet)
	{
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
			.AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
			);
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
			.AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnMaxHealthChanged.Broadcast(Data.NewValue);
				}
			);

		OnHealthChanged.Broadcast(AttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GetAuraASC()->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnASCRegistered.Broadcast(AbilitySystemComponent);
}
