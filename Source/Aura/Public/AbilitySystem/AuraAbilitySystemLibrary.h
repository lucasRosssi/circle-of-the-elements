// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Enums/TargetTeam.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

class ULoadoutWidgetController;
enum class ECharacterType : uint8;
class UMainMenuWidgetController;
struct FAuraAbilityInfo;
struct FAuraUpgradeInfo;
class UUpgradeMenuWidgetController;
class URewardsInfo;
struct FEffectParams;
class URegionInfo;
enum class ETargetTeam : uint8;
class UStatusEffectInfo;
struct FAbilityParams;
class UActiveDamageAbility;
class UBaseAbility;
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
  UFUNCTION(
    BlueprintPure,
    Category = "AuraAbilitySystemLibrary|WidgetController",
    meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
  )
  static UUpgradeMenuWidgetController* GetUpgradeMenuWidgetController(
    const UObject* WorldContextObject
  );
  UFUNCTION(
    BlueprintPure,
    Category = "AuraAbilitySystemLibrary|WidgetController",
    meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
  )
  static ULoadoutWidgetController* GetLoadoutWidgetController(
    const UObject* WorldContextObject
  );
  UFUNCTION(
    BlueprintPure,
    Category = "AuraAbilitySystemLibrary|WidgetController",
    meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject")
  )
  static UMainMenuWidgetController* GetMainMenuWidgetController(
    const UObject* WorldContextObject
  );

	/*
	 * DEFAULTS
	 */

	// UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	// static void GiveStartupAbilities(
	// 	const UObject* WorldContextObject,
	// 	UAbilitySystemComponent* ASC
	// );

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
	static TArray<FEffectParams> GetStatusEffects(const FGameplayEffectContextHandle& EffectContextHandle);

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
	static void SetStatusEffects(
		UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,
		const TArray<FEffectParams>& InEffectsArray
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
	static void GetAliveCharactersWithinBox(
		const AActor* ContextActor,
		TArray<AActor*>& OutOverlappingActors,
		UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
		const FVector& Dimensions,
		const FVector& Center,
		const FQuat& Rotation,
		ETargetTeam TargetTeam = ETargetTeam::Both
	);

  UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
  static void GetAliveCharactersWithinCone(
    const AActor* ContextActor,
    TArray<AActor*>& OutOverlappingActors,
    UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
    const FVector& Origin, 
    const FVector& Direction, 
    float Length, 
    float AngleWidth, 
    float AngleHeight, 
    ETargetTeam TargetTeam = ETargetTeam::Both
  );

  UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
  static void GetAliveCharactersWithinLine(
    const AActor* ContextActor,
    TArray<AActor*>& OutOverlappingActors,
    UPARAM(ref) TArray<AActor*>& ActorsToIgnore,
    const FVector& LineStart,
    const FVector& LineEnd,
    ETargetTeam TargetTeam = ETargetTeam::Both
  );

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetClosestActorToTarget(
		AActor* TargetActor,
		float Radius,
		ETargetTeam TargetTeam,
		UPARAM(ref) TArray<AActor*>& ActorsToIgnore
	);

	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetClosestTargetToPoint(
		AActor* Instigator,
		FVector Point,
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

	// If it has ANY tag that prevents harmful effects
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayMechanics")
	static bool HasAnyHarmfulEffectBlockTag(const UAbilitySystemComponent* TargetASC);

	// If it has tags that prevents harmful effects, but abilities still interact with target
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
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static FString GetAbilityDescription(
		const UAbilityInfo* AbilityInfo,
		const FGameplayTag& AbilityTag,
		int32 Level,
		bool bNextLevel
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static FString GetAbilityNextLevelDescription(
		const UAbilityInfo* AbilityInfo,
		const FGameplayTag& AbilityTag,
		int32 Level
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static void FormatAbilityDescriptionAtLevel(
		const FAuraAbilityInfo& AbilityInfo,
		int32 Level,
		FText& OutDescription,
		bool bNextLevel
		);
  UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
  static void FormatUpgradeDescriptionAtLevel(
    const FAuraUpgradeInfo& UpgradeInfo,
    int32 Level,
    FText& OutDescription
    );
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static FString GetAbilityLockedDescription(
		int32 Level,
		const FGameplayTagContainer& AbilitiesRequirement
	);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static void MakeAbilityDetailsText(
		const UBaseAbility* Ability,
		int32 Level,
		FString& OutManaText,
		FString& OutCooldownText,
		FString& OutChargesText
		);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|UI")
	static void MakeAbilityDetailsTextNextLevel(
		const UBaseAbility* Ability,
		int32 Level,
		FString& OutManaText,
		FString& OutCooldownText
		);

  UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Ability")
  static bool IsAbilityGranted(const UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag);
  UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Ability")
  static bool IsAbilityActive(const UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag);

private:
	static void AddOverlappedCharactersByTeam(
		const AActor* ContextActor,
		TArray<AActor*>& OutOverlappingActors,
		const TArray<FOverlapResult>& Overlaps,
		ETargetTeam TargetTeam
		);
  static void AddHitCharactersByTeam(
    const AActor* ContextActor,
    TArray<AActor*>& OutOverlappingActors,
    const TArray<FHitResult>& HitResults,
    ETargetTeam TargetTeam
    );
};
