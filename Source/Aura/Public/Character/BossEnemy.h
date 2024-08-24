// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraEnemy.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ABossEnemy : public AAuraEnemy
{
	GENERATED_BODY()

public:
	ABossEnemy();

	UFUNCTION(BlueprintCallable)
	void NextBossState();
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	void CheckHealth(float NewHealth);
	
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Boss|State",
		meta=(ClampMin=0.f, UIMin=0.f, ClampMax=1.f, UIMax=1.f)
		)
	TArray<float> BossStateHealthThresholds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boss|State")
	TArray<TSubclassOf<UGameplayEffect>> BossStateEffects;

private:
	int32 BossState = 0;
};
