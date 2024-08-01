// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/CharacterType.h"
#include "Data/CharacterInfo.h"
#include "Enums/TargetTeam.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

class URegionInfo;
enum class ECharacterName : uint8;
enum class ETargetTeam : uint8;
class UStatusEffectInfo;
struct FAbilityParams;
class UActiveDamageAbility;
class UBaseAbility;
class AAuraHUD;
class USkillMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;

USTRUCT(BlueprintType)
struct FRadialProps
{
	GENERATED_BODY()
	
	float InnerRadius = 0.f;
	float OuterRadius = 0.f;
	FVector Origin = FVector::ZeroVector;
};
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
		Category = "AuraAbilitySystemLibrary"
	)
	static UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent(AActor* Actor);
	
	/*
	 * WIDGET CONTROLLER
	 */
		
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

	/*
	 * DEFAULTS
	 */

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilitiesInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|StatusEffectsDefaults")
	static UStatusEffectInfo* GetStatusEffectInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|EncounterDefaults")
	static URegionInfo* GetEncounterInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(
		const UObject* WorldContextObject,
		UAbilitySystemComponent* ASC
	);

	/*
	 * GAMEPLAY EFFECTS
	 */

	// EFFECT CONTEXT GETTERS

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsParried(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsApplyingEffect(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetEffectValue(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetEffectDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetEffectType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetForwardVector(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetApplyHitReact(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static FRadialProps GetRadialProps(const FGameplayEffectContextHandle& EffectContextHandle);

	// EFFECT CONTEXT SETTERS

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsParried(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		bool bInParried
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		bool bInCriticalHit
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsApplyingEffect(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		bool bInApplyingEffect
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetEffectValue(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		float InEffectValue
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetEffectDuration(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		float InEffectDuration
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetEffectType(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		const FGameplayTag& InEffectType
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetForwardVector(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		const FVector& InForce
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetApplyHitReact(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		bool bInApplyHitReact
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsAreaAbility(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		bool bInIsRadialDamage
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetAreaInnerRadius(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		float InInnerRadius
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetAreaOuterRadius(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		float InOuterRadius
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetAreaOrigin(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		const FVector& InOrigin
		);

	/*
	 * GAMEPLAY MECHANICS
	 */

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void StackEncounterXP(const UObject* WorldContextObject, int32 InXP);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyAbilityEffect(
		const FAbilityParams& AbilityParams,
		bool& bSuccess
		);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static float GetRadialDamageWithFalloff(
		const AActor* TargetActor,
		float BaseDamage,
		float MinimumDamage,
		const FVector& Origin,
		float DamageInnerRadius,
		float DamageOuterRadius,
		float DamageFalloff
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetAliveCharactersWithinRadius(
		const AActor* ContextActor,
		TArray<AActor*>& OutOverlappingActors,
		UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
		float Radius,
		const FVector& SphereOrigin,
		ETargetTeam TargetTeam = ETargetTeam::Both
	);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetClosestActorToTarget(
		AActor* TargetActor,
		float Radius,
		ETargetTeam TargetTeam,
		UPARAM(ref) TArray<AActor*>& ActorsToIgnore
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsEnvironment(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool AreActorsFriends(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetFriendsWithinRadius(
		AActor* TargetActor,
		TArray<AActor*>& OutFriends,
		float Radius,
		const FVector& SphereOrigin
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetFriendsWithinTrace(
		AActor* TargetActor,
		TArray<AActor*>& OutFriends,
		const FVector& Start, 
		const FVector& End, 
		float Radius
		);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool AreActorsEnemies(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetEnemiesWithinRadius(
		AActor* TargetActor,
		TArray<AActor*>& OutEnemies,
		float Radius,
		const FVector& SphereOrigin
	);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<AActor*> GetAllTeamMembersFromActor(AActor* Actor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsPlayerUsingGamepad(const AActor* AvatarActor);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsTargetInvulnerable(AActor* TargetActor);
	
	static int32 GetXPRewardForTypeAndLevel(
		const UObject* WorldContextObject,
		ECharacterType CharacterType,
		int32 Level
	);

	// If has ANY tag that prevents harmful effects
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool HasAnyHarmfulEffectBlockTag(const UAbilitySystemComponent* TargetASC);

	// If has tags that prevents harmful effects, but abilities still interact with target
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool HasAnyParryTag(const UAbilitySystemComponent* TargetASC);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(
		const FVector& Forward,
		const FVector& Axis,
		float Spread,
		int32 Count
		);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(
		const FVector& Forward,
		const FVector& Axis,
		float Spread,
		int32 Count
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void ChangeActorTeam(
		int32 InTeamID,
		AActor* TargetActor,
		bool bPermanent = false
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void JoinToActorTeam(
		AActor* ActorToJoinTo,
		AActor* JoiningActor,
		bool bPermanent = false
		);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static void GoBackToOriginalTeam(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetActiveEffectCauser(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayTag& EffectTag
		);

	/*
	 * UI
	 */
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static FString GetAbilityDescription(
		const UAbilityInfo* AbilityInfo,
		const FGameplayTag& AbilityTag,
		int32 Level
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static FString GetAbilityNextLevelDescription(
		const UAbilityInfo* AbilityInfo,
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
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void MakeManaAndCooldownText(
		const UBaseAbility* Ability,
		int32 Level,
		FString& OutManaText,
		FString& OutCooldownText
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void MakeManaAndCooldownTextNextLevel(
		const UBaseAbility* Ability,
		int32 Level,
		FString& OutManaText,
		FString& OutCooldownText
		);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static TArray<FGameplayTag> GetAllAbilitiesFromLevel(
		const UObject* WorldContextObject,
		ECharacterName CharacterName,
		int32 Level
		);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static TArray<FGameplayTag> GetRandomAbilitiesFromLevel(
		const UObject* WorldContextObject,
		ECharacterName CharacterName,
		int32 Level,
		int32 Amount
		);
};
