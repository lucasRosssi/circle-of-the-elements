// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Game/AuraGameModeBase.h"
#include "AuraSystemComponent.generated.h"

class AAuraGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API UAuraSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAuraSystemComponent();

	void SetGameMode(AAuraGameModeBase* InGameMode) { AuraGameMode = InGameMode; }

protected:
	AAuraGameModeBase* GetAuraGameMode();
	TWeakObjectPtr<AAuraGameModeBase> AuraGameMode;

private:
		
};
