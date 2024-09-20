// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraHero.generated.h"

class AAuraCamera;
class USpotLightComponent;
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
	void DeathMontageEndRagdoll();

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
	virtual void SetInteractMessageVisible_Implementation(bool bVisible) override;
	virtual ECharacterName GetHeroName_Implementation() override;
	/** end Player Interface */
	
	void StartDeath();

	void EndDeath();

	UFUNCTION(BlueprintCallable)
	AAuraPlayerState* GetAuraPlayerState() const;
	UFUNCTION(BlueprintPure, meta=(DefaultToSelf="Target", HidePin="Target"))
	AAuraPlayerController* GetAuraPlayerController();

	UPROPERTY(EditDefaultsOnly, Category="Character Defaults|Abilities|Startup")
	TArray<TSubclassOf<UGameplayAbility>> EligibleAbilities;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<UNiagaraSystem> LevelUpNiagaraSystem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TObjectPtr<USoundBase> LevelUpSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LevelUpWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	TSubclassOf<AAuraCamera> CameraClass;
	UPROPERTY(BlueprintReadOnly, Category="Camera")
	TObjectPtr<AAuraCamera> ActiveCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpotLightComponent> SpotLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<UNiagaraSystem> DeathBloodEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> InteractWidgetComponent;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	bool bDying = false;

	TWeakObjectPtr<AAuraPlayerController> AuraPlayerController;
};
