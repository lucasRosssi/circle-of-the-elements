// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "Enums/CharacterType.h"
#include "CharacterInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class AURA_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Character Defaults")
	TMap<ECharacterType, FScalableFloat> XPReward;
};
