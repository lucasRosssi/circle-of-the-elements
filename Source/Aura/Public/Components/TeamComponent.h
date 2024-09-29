// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "TeamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTeamComponent();

	UFUNCTION(BlueprintPure, Category="Team")
	bool IsEnemy(AActor* TargetActor);
	UFUNCTION(BlueprintPure, Category="Team")
	bool IsFriend(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Team")
	void JoinActorTeam(AActor* Actor, bool bPermanent = false);

	UFUNCTION(BlueprintCallable, Category="Team")
	void ChangeTeam(int32 InTeamID, bool bPermanent = false);

	UFUNCTION(BlueprintCallable, Category="Team")
	void GoBackToOriginalTeam();

	// ID of the team this actor belongs to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Team")
	int32 TeamID;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category="Team")
	int32 OriginalTeamID;

private:
	
};
