// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "MeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMeleeAttackAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
	
protected:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Weapon",
		meta=(EditCondition="bUseWeapon", DisplayPriority=1)
		)
	bool bWithWeaponCollision;
	
};
