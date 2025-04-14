// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "WidgetInteractable.generated.h"

enum class ECharacterName : uint8;
class UAuraUserWidget;

UCLASS()
class AURA_API AWidgetInteractable : public AInteractable
{
  GENERATED_BODY()

public:
  virtual void Interact_Implementation(const AController* Controller) override;

protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TSubclassOf<UAuraUserWidget> WidgetClass;

private:
};
