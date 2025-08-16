// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterName.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

struct FGameplayTag;
class UInteractComponent;
class ATargetingActor;
enum class ETargetTeam : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, const AAuraPlayerController*, InstigatorController);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	static bool Implements(const UObject*);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	static void SafeExec_AddToXP(UObject* , int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePoints();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddAttributePoints(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSkillPoints();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddSkillPoints(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowTargetingActor(
		TSubclassOf<ATargetingActor> TargetingActorClass,
		ETargetTeam TargetTeam,
		float Radius
		);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideTargetingActor();

	UFUNCTION(BlueprintNativeEvent)
	void SetInteractMessageVisible(const FText& InteractText);
  static void Safe_SetInteractMessageVisible(UObject*, const FText& InteractText);

  UFUNCTION(BlueprintNativeEvent)
  void AddInteractableToList(UInteractComponent* InteractableComponent);
  static void Safe_AddInteractableToList(UObject*, UInteractComponent* InteractableComponent);
  UFUNCTION(BlueprintNativeEvent)
  void RemoveInteractableFromList(UInteractComponent* InteractableComponent);
  static void Safe_RemoveInteractableFromList(UObject*, UInteractComponent* InteractableComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterName GetHeroName();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void AddResource(const FGameplayTag& ResourceTag, int32 Amount);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void SpendAttributePointsRandomly();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void AddAttribute(const FGameplayTag& AttributeTag, int32 Amount);
};
