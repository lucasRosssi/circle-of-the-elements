// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void SetMovementPlayRate(float InPlayRate);
	void SetActionPlayRate(float InPlayRate);
  void SetStateTag(const FGameplayTag& InStateTag) { StateTag = InStateTag; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementPlayRate = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ActionPlayRate = 1.f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FGameplayTag StateTag;
};
