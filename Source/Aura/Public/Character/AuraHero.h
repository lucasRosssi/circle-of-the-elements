// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "AuraHero.generated.h"

class UOrbitManagerComponent;
class UAuraSaveGame;
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

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

  virtual void InitializeAbilities() override;
  void InitializeUpgrades();
	
	/** Combat Interface */
	virtual void Die(const FVector& DeathImpulse) override;
  virtual void SetCustomDepth_Implementation(int32 Value) override;
  virtual int32 GetCharacterLevel_Implementation() override;
  virtual TArray<USpirit*> GetEquippedSpirits_Implementation() override;
  virtual TArray<USpirit*> GetAvailableEquippedSpirits_Implementation() override;
	/** end Combat Interface */
	void DeathMontageEndRagdoll();

	/** Player Interface */
  virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual void AddAttributePoints_Implementation(int32 Amount) override;
	virtual int32 GetSkillPoints_Implementation() override;
	virtual void AddSkillPoints_Implementation(int32 Amount) override;
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
  virtual void AddResource_Implementation(const FGameplayTag& ResourceTag, int32 Amount) override;
  virtual void SpendAttributePointsRandomly_Implementation() override;
  virtual void AddAttribute_Implementation(const FGameplayTag& AttributeTag, int32 Amount) override;
	/** end Player Interface */
	
	void StartDeath();
	void EndDeath();

  void BackToHome();
  
	UFUNCTION(BlueprintPure, meta=(DefaultToSelf="Target", HidePin="Target"))
	AAuraPlayerController* GetAuraPlayerController();

  AAuraCamera* GetActiveCamera() const { return ActiveCamera; }

protected:
	virtual void BeginPlay() override;

  virtual void InitializeAttributes() override;

  UFUNCTION(BlueprintImplementableEvent)
  void SetInteractionWidgetText(const FText& Text);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UOrbitManagerComponent> OrbitManager;
  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	TSubclassOf<AAuraCamera> CameraClass;
	UPROPERTY(BlueprintReadOnly, Category="Camera")
	TObjectPtr<AAuraCamera> ActiveCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<UNiagaraSystem> DeathBloodEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	TObjectPtr<USoundBase> DeathSound2;
  UPROPERTY(EditDefaultsOnly, Category="Death")
  float BackToHomeDelay = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<UWidgetComponent> InteractWidgetComponent;


private:
	virtual void InitAbilityActorInfo() override;

  UFUNCTION()
  void SaveCurrentHealth();

  AAuraPlayerState* GetAuraPlayerState();

  UAuraSaveGame* GetSaveGame();

  UPROPERTY()
  TObjectPtr<AAuraPlayerState> AuraPlayerState;
  
	TWeakObjectPtr<AAuraPlayerController> AuraPlayerController;
  
	bool bDying = false;

  UPROPERTY()
  TObjectPtr<UAuraSaveGame> SaveGame;
  
};
