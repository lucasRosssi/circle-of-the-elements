// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "ActiveDamageAbility.h"
#include "MeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMeleeAttackAbility : public UActiveDamageAbility
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
  
  virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
  virtual void HandleComboSequence() override;
  
	UFUNCTION(BlueprintCallable)
	void ExecuteAttackScan();
	UFUNCTION(BlueprintCallable)
	void HandleWeaponCollision();
	
protected:
  virtual FComboData GetComboData() const override;

  UFUNCTION(BlueprintPure, meta=(DefaultToSelf="Target", HidePin="Target"))
  EMeleeHitMode GetMeleeHitMode() const;
  FVector GetShapeOffset() const;
  FVector GetBoxHitDimensions() const;
  float GetSphereHitRadius() const;
  float GetAngleWidth() const;
  float GetAngleHeight() const;
  
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode"
		)
	EMeleeHitMode MeleeHitMode = EMeleeHitMode::Sphere;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Mode",
    meta=(EditCondition="MeleeHitMode != EMeleeHitMode::Weapon", EditConditionHides)
    )
  FVector ShapeOffset = FVector::ZeroVector;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Box", EditConditionHides)
		)
	FVector BoxHitDimensions = FVector::ZeroVector;
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category="Ability Defaults|Mode",
		meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Sphere", EditConditionHides)
		)
	float SphereHitRadius = 100.0f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Mode",
    meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Cone", EditConditionHides)
    )
  float AngleWidth = 120.0f;
  UPROPERTY(
    EditDefaultsOnly,
    BlueprintReadOnly,
    Category="Ability Defaults|Mode",
    meta=(EditCondition="MeleeHitMode == EMeleeHitMode::Cone", EditConditionHides)
    )
  float AngleHeight = 60.f;

private:
	void DrawDebugMeleeHitShape(const FVector& AbilitySocketLocation);
	TArray<AActor*> ShapeScan(const FVector& Origin);
	void ApplyEffectToHitActor(AActor* HitActor);
	UFUNCTION()
	void OnWeaponCollision(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

  UPROPERTY()
  TArray<AActor*> ActorsHitInMontage;
};
