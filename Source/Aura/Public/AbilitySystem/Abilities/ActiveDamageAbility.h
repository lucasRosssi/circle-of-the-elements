// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveAbility.h"
#include "Enums/MeleeHitMode.h"
#include "ActiveDamageAbility.generated.h"

USTRUCT(BlueprintType)
struct FComboData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TObjectPtr<UAnimMontage> Montage;

  // If this montage has its own properties instead of using the ability defaults
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  bool bUseCustomData = false;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseCustomData", EditConditionHides))
  bool bUseWeaponSocket = false;
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bUseCustomData", EditConditionHides))
  FName SocketName = NAME_None;

  UPROPERTY()
  bool bMeleeAbility = false;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly, meta=(EditCondition="bUseCustomData && bMeleeAbility", EditConditionHides)
    )
  EMeleeHitMode MeleeHitMode = EMeleeHitMode::Sphere
;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(EditCondition="bUseCustomData && bMeleeAbility && MeleeHitMode != EMeleeHitMode::Weapon", EditConditionHides)
    )
  FVector ShapeOffset = FVector::ZeroVector
;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(EditCondition="bUseCustomData && bMeleeAbility && MeleeHitMode == EMeleeHitMode::Box", EditConditionHides)
    )
  FVector BoxHitDimensions = FVector::ZeroVector;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(EditCondition="bUseCustomData && bMeleeAbility && MeleeHitMode == EMeleeHitMode::Sphere", EditConditionHides)
    )
  float SphereHitRadius = 100.0f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(EditCondition="bUseCustomData && bMeleeAbility && MeleeHitMode == EMeleeHitMode::Cone", EditConditionHides)
    )
  float AngleWidth = 120.0f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    meta=(EditCondition="bUseCustomData && bMeleeAbility && MeleeHitMode == EMeleeHitMode::Cone", EditConditionHides)
    )
  float AngleHeight = 45.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UActiveDamageAbility : public UActiveAbility
{
  GENERATED_BODY()

public:
  virtual FAbilityParams MakeAbilityParamsFromDefaults(AActor* TargetActor = nullptr) const override;
  virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                          const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                          bool bWasCancelled) override;
  
  // Ability Interface overrides
  virtual bool IsDamageAbility_Implementation() const override;
  virtual FGameplayTag GetDamageTypeTag_Implementation() const override;;
  // END Ability Interface overrides

  UFUNCTION(BlueprintCallable)
  FAbilityParams ApplyEffectChangePerHitToAbilityParams(
    UPARAM(ref) FAbilityParams& AbilityParams,
    int32 HitCount
  );

  UFUNCTION(BlueprintPure, Category="Ability Defaults|Damage")
  float GetDamageAtLevel(int32 Level) const;
  UFUNCTION(BlueprintPure, Category="Ability Defaults|Damage")
  virtual int32 GetRoundedDamageAtLevel_Implementation(int32 Level) const override;

  UFUNCTION(BlueprintCallable, Category="Ability Defaults|Combo")
  virtual void HandleComboSequence();
  UFUNCTION(BlueprintImplementableEvent, Category="Ability Defaults|Combo")
  void OnNextComboSequence();

protected:
  // Active Ability overrides
  UFUNCTION(BlueprintPure, Category="AbilityDefaults", meta=(DefaultToSelf="Target", HidePin="Target"))
  virtual UAnimMontage* GetAnimMontage() const override;
  // END Active Ability overrides
  
  UFUNCTION(BlueprintPure, Category="Ability Defaults", meta=(DefaultToSelf="Target", HidePin="Target"))
  FName GetAbilitySocketName() const;
  UFUNCTION(BlueprintPure, Category="Ability Defaults", meta=(DefaultToSelf="Target", HidePin="Target"))
  bool IsUsingWeapon() const;

  UFUNCTION(BlueprintPure, Category="Ability Defaults|Combo", meta=(DefaultToSelf="Target", HidePin="Target"))
  virtual FComboData GetComboData() const;
  UFUNCTION(BlueprintPure, Category="Ability Defaults|Combo", meta=(DefaultToSelf="Target", HidePin="Target"))
  float GetComboMagnitude() const;
  UFUNCTION(BlueprintPure, Category="Ability Defaults|Combo", meta=(DefaultToSelf="Target", HidePin="Target"))
  float GetComboFinishMagnitude() const;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
  TSubclassOf<UGameplayEffect> DamageEffectClass;

  UPROPERTY(EditDefaultsOnly, Category="Damage", meta=( Categories="Damage" ))
  FGameplayTag DamageType;

  UPROPERTY(EditDefaultsOnly, Category="Damage")
  FScalableFloat Damage;

  UPROPERTY(EditDefaultsOnly, Category="Status Effects")
  bool bApplyHitReact = true;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Defaults|Combo")
  bool bIsComboSequence = false;

  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Combo",
    meta=(EditCondition="bIsComboSequence")
  )
  TArray<FComboData> ComboSequenceData;

  // How much each combo step increases/decrease the ability effect
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Combo",
    meta=(EditCondition="bIsComboSequence")
  )
  FScalableFloat ComboMagnitude = 0.f;

  // How much the last combo step increases/decrease the ability effect
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Combo",
    meta=(EditCondition="bIsComboSequence")
  )
  FScalableFloat ComboFinishMagnitude = 0.f;

  UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Combo")
  bool bComboInputPressed = false;

  UPROPERTY(BlueprintReadWrite, Category="Ability Defaults|Combo")
  int32 ComboIndex = 0;
};
