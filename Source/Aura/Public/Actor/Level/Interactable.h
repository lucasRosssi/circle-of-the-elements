// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class AAuraPlayerController;
class UNiagaraSystem;
class UNiagaraComponent;
enum class ECharacterName : uint8;
class AAuraGameModeBase;
class USphereComponent;

UCLASS()
class AURA_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	UFUNCTION()
	void OnInteractAreaOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void OnInteractAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		);

	float GetInteractAreaRadius();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void EnableInteraction();
	UFUNCTION()
	void DisableInteraction();

	UFUNCTION()
	void PreInteract(AAuraPlayerController* InstigatorController);
	virtual void Interact(AAuraPlayerController* InstigatorController);
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteracted(AAuraPlayerController* InstigatorController);

	UFUNCTION(BlueprintPure)
	AAuraGameModeBase* GetAuraGameMode();
	
	UPROPERTY()
	AAuraGameModeBase* AuraGameMode = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<USphereComponent> InteractArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	bool bInteractionEnabled = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<USoundBase> InteractSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TMap<ECharacterName, TObjectPtr<UAnimMontage>> InteractMontages;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Spawn")
	TObjectPtr<UNiagaraSystem> SpawnNiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Idle")
	TObjectPtr<UNiagaraSystem> IdleNiagaraSystem;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Interact")
	TObjectPtr<UNiagaraSystem> InteractNiagaraSystem;
	UPROPERTY()
	UNiagaraComponent* IdleNiagaraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Spawn")
	FVector SpawnNiagaraOffset = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Idle")
	FVector IdleNiagaraOffset = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Niagara|Interact")
	FVector InteractNiagaraOffset = FVector::ZeroVector;
private:

};
