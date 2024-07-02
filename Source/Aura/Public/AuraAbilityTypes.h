#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bApplyHitReact = false;

	bool IsValid() const
	{
		return DamageEffectClass != nullptr;
	}
};

USTRUCT(BlueprintType)
struct FEffectParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag GameplayTag = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float Value = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float Duration = 0.f;

	bool IsValid() const
	{
		return GameplayEffectClass != nullptr && GameplayTag.IsValid();
	}
};

USTRUCT(BlueprintType)
struct FAbilityParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	
	UPROPERTY(BlueprintReadWrite)
	int32 AbilityLevel = 1;

	UPROPERTY(BlueprintReadWrite)
	FDamageParams DamageParams = FDamageParams();

	UPROPERTY(BlueprintReadWrite)
	FEffectParams EffectParams = FEffectParams();

	UPROPERTY(BlueprintReadWrite)
	FVector ForwardVector = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAreaAbility = false;

	UPROPERTY(BlueprintReadWrite)
	float AreaInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AreaOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector AreaOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bCriticalHit; }
	bool IsParried() const { return bParried; }
	bool IsApplyingEffect() const { return bApplyingEffect; }
	float GetEffectValue() const { return EffectValue; }
	float GetEffectDuration() const { return EffectDuration; }
	TSharedPtr<FGameplayTag> GetEffectType() const { return EffectType; }
	FVector GetForwardVector() const { return ForwardVector; }
	bool GetApplyHitReact() const { return bApplyHitReact; }
	bool IsAreaAbility() const { return bIsAreaAbility; }
	float GetAreaInnerRadius() const { return AreaInnerRadius; }
	float GetAreaOuterRadius() const { return AreaOuterRadius; }
	FVector GetAreaOrigin() const { return AreaOrigin; }

	void SetIsCriticalHit(bool bInCriticalHit) { bCriticalHit = bInCriticalHit; }
	void SetIsParried(bool bInParried) { bParried = bInParried; }
	void SetIsApplyingEffect(bool bInApplyingEffect) { bApplyingEffect = bInApplyingEffect; }
	void SetEffectValue(float InEffectValue) { EffectValue = InEffectValue; }
	void SetEffectDuration(float InEffectDuration) { EffectDuration = InEffectDuration; }
	void SetEffectType(TSharedPtr<FGameplayTag> InEffectType) { EffectType = InEffectType; }
	void SetForwardVector(const FVector& InVector) { ForwardVector = InVector; }
	void SetApplyHitReact(bool InApplyHitReact) { bApplyHitReact = InApplyHitReact; }
	void SetIsAreaAbility(bool bInIsRadialDamage) { bIsAreaAbility = bInIsRadialDamage; }
	void SetAreaInnerRadius(float InInnerRadius) { AreaInnerRadius = InInnerRadius; }
	void SetAreaOuterRadius(float InOuterRadius) { AreaOuterRadius = InOuterRadius; }
	void SetAreaOrigin(const FVector& InOrigin) { AreaOrigin = InOrigin; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/* Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bParried = false;
	UPROPERTY()
	bool bCriticalHit = false;
	UPROPERTY()
	bool bDoT = false;
	UPROPERTY()
	bool bApplyingEffect = false;
	UPROPERTY()
	float EffectValue = 0.f;
	UPROPERTY()
	float EffectDuration = 0.f;
	
	TSharedPtr<FGameplayTag> EffectType;

	UPROPERTY()
	FVector ForwardVector = FVector::ZeroVector;

	UPROPERTY()
	bool bApplyHitReact = true;

	UPROPERTY()
	bool bIsAreaAbility = false;

	UPROPERTY()
	float AreaInnerRadius = 0.f;

	UPROPERTY()
	float AreaOuterRadius = 0.f;

	UPROPERTY()
	FVector AreaOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
