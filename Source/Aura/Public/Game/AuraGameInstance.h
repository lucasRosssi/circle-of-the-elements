// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
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
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player")
	ECharacterName CurrentCharacter = ECharacterName::Aura;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Run")
	int32 EncountersCount = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Run")
	TArray<ULevel*> LevelsTaken;
	
private:
};
