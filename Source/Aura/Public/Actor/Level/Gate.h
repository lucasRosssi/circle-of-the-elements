// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Actor/Level/Interactable.h"
#include "Enums/GatePosition.h"
#include "Enums/Region.h"
#include "Gate.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AGate : public AInteractable
{
	GENERATED_BODY()

public:
	AGate();
	
protected:
	virtual void BeginPlay() override;

	// START Interactable overrides
	virtual void Interact() override;
	// END Interactable overrides

	UFUNCTION(BlueprintCallable)
	void GoToRandomLocation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GateMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Location")
	ERegion Region = ERegion::Undefined;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Location")
	EGatePosition NextGatePosition = EGatePosition::BottomLeft;
private:
};
