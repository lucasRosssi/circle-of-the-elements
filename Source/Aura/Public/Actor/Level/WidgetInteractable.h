// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WidgetInteractable.generated.h"

enum class ECharacterName : uint8;
class AAuraPlayerController;
class UAuraUserWidget;
class UInteractComponent;

UCLASS()
class AURA_API AWidgetInteractable : public AActor
{
  GENERATED_BODY()

public:
  AWidgetInteractable();

protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> StaticMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UInteractComponent> InteractComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
  TSubclassOf<UAuraUserWidget> WidgetClass;

private:
  void Interact(const AAuraPlayerController* Controller);

  ECharacterName CharacterName;
};
