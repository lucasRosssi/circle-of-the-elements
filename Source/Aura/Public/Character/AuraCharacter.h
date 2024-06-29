// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class AMainPlayerController;
class AAuraPlayerState;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() override;
	/** end Combat Interface */

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSkillPoints_Implementation() const override;
	virtual void SpendAttributePoints_Implementation(int32 Amount) override;
	virtual void SpendSkillPoints_Implementation(int32 Amount) override;
	virtual void ShowTargetingActor_Implementation(TSubclassOf<ATargetingActor> TargetingActorClass) override;
	virtual void HideTargetingActor_Implementation() override;
	/** end Player Interface */

	UFUNCTION(BlueprintCallable)
	AAuraPlayerState* GetAuraPlayerState() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<UNiagaraSystem> LevelUpNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<USoundBase> LevelUpSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LevelUpWidgetComponent;

	UPROPERTY(BlueprintReadOnly)
	AMainPlayerController* MainPlayerController;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
};
