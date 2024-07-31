// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AttributeSetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IAttributeSetInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();
	virtual void PossessedBy(AController* NewController) override;
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	void SetLevel(int32 InLevel) { Level = InLevel; }
	int32 GetMyLevel() const { return Level; }

	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual void Die(const FVector& DeathImpulse = FVector::ZeroVector) override;
	/** end Combat Interface */

	/* Attribute Set Interface */
	virtual void SetActionSpeed_Implementation(float InActionSpeed) override;
	virtual float GetPower_Implementation() override;
	virtual void SetTimeDilation_Implementation(float InTimeDilation) override;
	/* END Attribute Set Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
