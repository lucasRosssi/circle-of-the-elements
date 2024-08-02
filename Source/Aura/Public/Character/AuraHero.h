// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraHero.generated.h"

class APostProcessVolume;
enum class ETargetTeam : uint8;
class AAuraPlayerController;
class AAuraPlayerState;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHero : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraHero();

	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void AddCharacterAbilities() override;
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual void Die(const FVector& DeathImpulse) override;
	/** end Combat Interface */

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSkillPoints_Implementation() const override;
	virtual void SpendAttributePoints_Implementation(int32 Amount) override;
	virtual void SpendSkillPoints_Implementation(int32 Amount) override;
	virtual void ShowTargetingActor_Implementation(
		TSubclassOf<ATargetingActor> TargetingActorClass,
		ETargetTeam TargetTeam,
		float Radius = 300.f
		) override;
	virtual void HideTargetingActor_Implementation() override;
	virtual FOnInteract& GetOnInteractDelegate() override;
	/** end Player Interface */
	
	void StartDeath();

	void EndDeath();

	UFUNCTION(BlueprintCallable)
	AAuraPlayerState* GetAuraPlayerState() const;
	AAuraPlayerController* GetAuraPlayerController();

	UPROPERTY(EditDefaultsOnly, Category="Character Defaults|Abilities|Startup")
	TArray<TSubclassOf<UGameplayAbility>> EligibleAbilities;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<UNiagaraSystem> LevelUpNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<USoundBase> LevelUpSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LevelUpWidgetComponent;

	UPROPERTY(BlueprintReadOnly)
	AAuraPlayerController* AuraPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<UNiagaraSystem> DeathBloodEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound2;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	bool bDying = false;
};
