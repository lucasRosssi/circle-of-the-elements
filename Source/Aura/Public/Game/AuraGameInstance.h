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

	void AddPlayerResource(const FGameplayTag& ResourceTag, int32 Amount);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player")
	ECharacterName CurrentCharacter = ECharacterName::Aura;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category="Player",
		meta=(Categories="Resources", ForceInlineRow)
		)
	TMap<FGameplayTag, int32> PlayerResources;

private:
};
