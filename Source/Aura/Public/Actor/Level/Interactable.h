// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

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

	virtual void Interact(AActor* InInstigator);
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteracted(AActor* InInstigator);

	UFUNCTION(BlueprintPure)
	AAuraGameModeBase* GetAuraGameMode();
	
	UPROPERTY()
	AAuraGameModeBase* AuraGameMode = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<USphereComponent> InteractArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	bool bInteractionEnabled = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	TObjectPtr<USoundBase> InteractSound;
private:
	UFUNCTION()
	void PreInteract(AActor* InInstigator);
};
