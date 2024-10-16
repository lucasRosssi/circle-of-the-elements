// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UHealTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealText(
		float Damage,
		bool bIsPlayer
	);
};
