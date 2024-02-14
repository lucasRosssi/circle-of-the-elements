// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindNearestTarget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTService_FindNearestTarget : public UBTService
{
	GENERATED_BODY()

	UBTService_FindNearestTarget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
};
