// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterType.h"
#include "Data/CharacterInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UActiveDamageAbility;
class UBaseAbility;
class AAuraHUD;
class USkillMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static FWidgetControllerParams MakeWidgetControllerParams(const UObject* WorldContextObject);
	
	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(
		const UObject* WorldContextObject
	);

	UFUNCTION(
		BlueprintPure,
		Category = "AuraAbilitySystemLibrary|WidgetController",
		meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static USkillMenuWidgetController* GetSkillMenuWidgetController(
		const UObject* WorldContextObject
	);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilitiesInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(
		const UObject* WorldContextObject,
		ECharacterClass CharacterClass,
		float Level,
		UAbilitySystemComponent* ASC
	);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(
		const UObject* WorldContextObject,
		UAbilitySystemComponent* ASC,
		ECharacterClass CharacterClass
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsParried(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsParried(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInParried);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetAliveCharactersWithinRadius(
		const UObject* WorldContextObject,
		TArray<AActor*>& OutOverlappingActors,
		UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
		float Radius,
		const FVector& SphereOrigin
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool AreActorsFriends(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool AreActorsEnemies(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsPlayerUsingGamepad(const AActor* AvatarActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsTargetInvulnerable(AActor* TargetActor);
	
	static int32 GetXPRewardForTypeAndLevel(
		const UObject* WorldContextObject,
		ECharacterType CharacterType,
		int32 Level
	);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static FString GetAbilityDescription(
		const UObject* WorldContextObject,
		const FGameplayTag& AbilityTag,
		int32 Level
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static FString GetAbilityNextLevelDescription(
		const UObject* WorldContextObject,
		const FGameplayTag& AbilityTag,
		int32 Level
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static void FormatAbilityDescriptionAtLevel(
		UBaseAbility* Ability,
		int32 Level,
		FText& OutDescription
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static FString GetAbilityLockedDescription(
		int32 Level,
		const FGameplayTagContainer& AbilitiesRequirement
	);
};
