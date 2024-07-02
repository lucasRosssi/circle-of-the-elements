// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

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
	int32 GetCharacterLevel();

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
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyForce(const FVector& InForce);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetTopStatusEffectSceneComponent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetBottomStatusEffectSceneComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCapsuleComponent* EnableWeaponCollision(bool bEnable);
	
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeath& GetOnDeathDelegate() = 0;
};
