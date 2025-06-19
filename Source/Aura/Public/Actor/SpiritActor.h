// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SpiritActor.generated.h"

class USpringArmComponent;

UCLASS()
class AURA_API ASpiritActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpiritActor();

public:
  void OnAbilityUse();
  
  void SetAbilityTag(const FGameplayTag& InTag);
  void SetCooldownTag(const FGameplayTag& InTag);
protected:
	virtual void BeginPlay() override;

private:
  FGameplayTag AbilityTag;
  FGameplayTag CooldownTag;

};
