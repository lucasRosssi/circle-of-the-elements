// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Decorator.generated.h"

UCLASS()
class AURA_API ADecorator : public AActor
{
	GENERATED_BODY()
	
public:	
	ADecorator();

protected:
	virtual void BeginPlay() override;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<USceneComponent> Root;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Wall1;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Wall2;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Wall3;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Wall4;

private:	


};
