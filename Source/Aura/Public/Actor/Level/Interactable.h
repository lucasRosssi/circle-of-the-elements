// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

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

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void EnableInteraction();
	UFUNCTION()
	void DisableInteraction();

	virtual void Interact(AController* InstigatorController);
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteracted(AController* InstigatorController);

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
private:
	UFUNCTION()
	void PreInteract(AController* InstigatorController);
};
