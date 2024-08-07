// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class ATargetingActor;
enum class ETargetTeam : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AActor*, Instigator);

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

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void SpendAttributePoints(int32 Amount);

	UFUNCTION(BlueprintNativeEvent)
	void SpendSkillPoints(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowTargetingActor(
		TSubclassOf<ATargetingActor> TargetingActorClass,
		ETargetTeam TargetTeam,
		float Radius
		);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideTargetingActor();

	UFUNCTION(BlueprintNativeEvent)
	void SetInteractMessageVisible(bool bVisible);

	virtual FOnInteract& GetOnInteractDelegate() = 0;
};
