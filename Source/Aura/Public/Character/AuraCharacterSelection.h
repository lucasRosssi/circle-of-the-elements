// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterName.h"
#include "GameFramework/Character.h"
#include "Interfaces/TargetInterface.h"
#include "AuraCharacterSelection.generated.h"

class AAuraHero;

UCLASS()
class AURA_API AAuraCharacterSelection : public ACharacter, public ITargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterSelection();

	/** Target Interface */
	virtual void HighlightActor_Implementation(AActor* InstigatorActor = nullptr) override;
	virtual void UnHighlightActor_Implementation() override;
	/** end Target Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hero Information")
	ECharacterName HeroName = ECharacterName::Undefined;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Hero Information")
	TSubclassOf<AAuraHero> HeroClass;
};
