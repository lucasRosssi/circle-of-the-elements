// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
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
	virtual void SetInteractMessageVisible_Implementation(const FText& InteractText) override;
  virtual void AddInteractableToList_Implementation(const UInteractComponent* InteractableComponent) override;
  virtual void RemoveInteractableFromList_Implementation(const UInteractComponent* InteractableComponent) override;
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

  UFUNCTION(BlueprintImplementableEvent)
  void SetInteractionWidgetText(const FText& Text);
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<UWidgetComponent> InteractWidgetComponent;

private:
	virtual void InitAbilityActorInfo() override;

	bool bDying = false;

	TWeakObjectPtr<AAuraPlayerController> AuraPlayerController;
};
