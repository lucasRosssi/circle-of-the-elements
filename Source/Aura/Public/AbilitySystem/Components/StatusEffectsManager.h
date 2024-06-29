// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StatusEffectsManager.generated.h"

enum class EStatusEffectPosition : uint8;
class UStatusEffectInfo;
class UNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API UStatusEffectsManager : public UActorComponent
{
	GENERATED_BODY()
public:
	UStatusEffectsManager();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Niagara")
	TMap<FGameplayTag, UNiagaraComponent*> ActiveNiagaraComponents;

protected:
	virtual void BeginPlay() override;
	void RegisterStatusEffectTagEvent(UAbilitySystemComponent* InASC);
	
	void StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

private:
	void OnActivateStatusEffect(const FGameplayTag& StatusEffectTag);
	void OnDeactivateStatusEffect(const FGameplayTag& StatusEffectTag);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateStatusEffect(
		const FGameplayTag& StatusEffectTag,
		UNiagaraSystem* StatusEffectNiagara,
		EStatusEffectPosition Position
		);
};
