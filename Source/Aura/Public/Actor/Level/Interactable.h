// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "Interactable.generated.h"

UCLASS()
class AURA_API AInteractable : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AInteractable();
  
  /* Interact Interface */
  virtual UInteractComponent* GetInteractComponent_Implementation() const override;
  /* END Interact Interface */

protected:
  virtual void BeginPlay() override;
  
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> StaticMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  bool bActive = true;
public:	
};
