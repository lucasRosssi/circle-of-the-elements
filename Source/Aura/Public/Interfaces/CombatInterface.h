// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UBoxComponent;
class UCapsuleComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetAvatarMesh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetAbilitySocketLocation(const FName SocketName, bool bSocketInWeapon = true);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetStunMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<UAnimMontage*> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetDodgeMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USoundBase* GetHitSound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterType GetCharacterType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyKnockback(const FVector& KnockbackForce);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetTopStatusEffectSceneComponent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetBottomStatusEffectSceneComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBoxComponent* EnableWeaponCollision(bool bEnable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsFriend(AActor* Actor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsEnemy(AActor* Actor);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void SetCustomDepth(int32 Value);

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  float GetCapsuleHalfHeight() const;
	
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeath& GetOnDeathDelegate() = 0;
  
};
