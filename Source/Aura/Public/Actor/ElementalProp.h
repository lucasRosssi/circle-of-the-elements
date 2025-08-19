// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ElementalProp.generated.h"

class UNiagaraComponent;

UCLASS()
class AURA_API AElementalProp : public AActor
{
	GENERATED_BODY()
	
public:	
	AElementalProp();

  void SetElementTag(const FGameplayTag& InElementTag);

  UFUNCTION(BlueprintImplementableEvent)
  void OnElementSet();

protected:
	virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> StaticMesh;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Elemental", meta=(Categories="Abilities.Element"))
  FGameplayTag ElementTag;

private:
  
};
