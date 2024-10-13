// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "Enums/CharacterName.h"
#include "AuraGameInstance.generated.h"

enum class ECharacterName : uint8;

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SaveHeroData();

	ECharacterName GetCurrentCharacterName() const { return CurrentCharacterName; };

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player")
	ECharacterName CurrentCharacterName = ECharacterName::Aura;


private:
};
