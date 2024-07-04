// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/StatusEffectPosition.h"
#include "StatusEffectInfo.generated.h"

enum class EStatusEffectPosition : uint8;
class UNiagaraSystem;
class UStatusEffect;

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStatusEffect> StatusEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="NiagaraSystem", EditConditionHides))
	EStatusEffectPosition Position = EStatusEffectPosition::Center;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( MultiLine=true ))
	FText Description = FText();

};

/**
 * 
 */
UCLASS()
class AURA_API UStatusEffectInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Status Effect Data",
		meta=(Categories="StatusEffects",	ForceInlineRow)
		)
	TMap<FGameplayTag, FStatusEffectData> StatusEffects;
};
