// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_ServiceFindNearestTarget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBT_ServiceFindNearestTarget : public UBTService
{
	GENERATED_BODY()

	UBT_ServiceFindNearestTarget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
};
