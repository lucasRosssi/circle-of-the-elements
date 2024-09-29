// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "WidgetInteractable.generated.h"

enum class ECharacterName : uint8;
class UAuraUserWidget;
class UInteractComponent;

UCLASS()
class AURA_API AWidgetInteractable : public AActor, public IInteractInterface
{
  GENERATED_BODY()

public:
  AWidgetInteractable();

  /* Interact Interface */
  virtual void Interact_Implementation(const AController* Controller) override;
  virtual UInteractComponent* GetInteractComponent_Implementation() const override;
  /* END Interact Interface */

protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> StaticMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TSubclassOf<UAuraUserWidget> WidgetClass;

private:
};
