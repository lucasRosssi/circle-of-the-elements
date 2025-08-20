// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Enums/StatusEffectPosition.h"
#include "StatusEffectInfo.generated.h"

class UNiagaraSystem;
class UAuraStatusEffect;

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAuraStatusEffect> StatusEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="NiagaraSystem != nullptr", EditConditionHides))
  bool bInWeapon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="!bInWeapon", EditConditionHides))
	EStatusEffectPosition Position = EStatusEffectPosition::Center;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bInWeapon", EditConditionHides))
  FName WeaponSocketName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor MeshTint = FLinearColor::Black;
	
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
