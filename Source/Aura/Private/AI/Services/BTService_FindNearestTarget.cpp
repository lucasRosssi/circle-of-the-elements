// Copyright Lucas Rossi


#include "AI/Services/BTService_FindNearestTarget.h"

#include "AIController.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FindNearestTarget::UBTService_FindNearestTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Find Nearest Target";
}

void UBTService_FindNearestTarget::TickNode(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds
)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<AActor*> ActorsToIgnore;
	AActor* ClosestActor = UAuraAbilitySystemLibrary::GetClosestActorToTarget(
		OwningPawn,
		2000.f,
		ETargetTeam::Enemies,
		ActorsToIgnore
		);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject(TargetToFollowSelector.SelectedKeyName, ClosestActor);
	BlackboardComponent->SetValueAsFloat(
		DistanceToTargetSelector.SelectedKeyName,
		OwningPawn->GetDistanceTo(ClosestActor)
		);
}
