// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

public:
	// ID of the team this actor belongs to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
	int32 TeamID;

	UFUNCTION(BlueprintCallable, Category="Team")
	bool IsEnemy(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Team")
	bool IsFriend(AActor* TargetActor);
};
