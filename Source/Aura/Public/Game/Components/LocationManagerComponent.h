// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AuraSystemComponent.h"
#include "Enums/Region.h"
#include "LocationManagerComponent.generated.h"


class AAuraGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AURA_API ULocationManagerComponent : public UAuraSystemComponent
{
	GENERATED_BODY()

public:	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Location")
	ERegion Region = ERegion::Undefined;

private:

};
