// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/AttributeSetInterface.h"
#include "AuraPlayerState.generated.h"

class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraCharacterBase;
class ULevelInfo;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */)

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface, public IAttributeSetInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAuraAbilitySystemComponent* GetAuraASC();
	UAuraAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/* Attribute Set Interface */
	virtual void SetMovementSpeed_Implementation(float InMovementSpeed) override;
	virtual void SetActionSpeed_Implementation(float InActionSpeed) override;
	virtual float GetDamageMultiplier_Implementation() override;
	virtual void SetTimeDilation_Implementation(float InTimeDilation) override;
	/* END Attribute Set Interface */

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelInfo> LevelInfo;
	
	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSkillPointsChangedDelegate;
	
	FORCEINLINE int32 GetCharacterLevel() const { return Level; }
	void SetLevel(int32 InLevel);
	void AddLevel(int32 InLevel);
	FORCEINLINE int32 GetXP() const { return XP; }
	void SetXP(int32 InXP);
	void AddXP(int32 InXP);
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	void SetAttributePoints(int32 InAttributePoints);
	void AddAttributePoints(int32 InAttributePoints);
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }
	void SetSkillPoints(int32 InSkillPoints);
	void AddSkillPoints(int32 InSkillPoints);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

private:
	AAuraCharacterBase* GetCharacterBase();
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SkillPoints)
	int32 SkillPoints = 0;
	UFUNCTION()
	void OnRep_SkillPoints(int32 OldSkillPoints);

	UPROPERTY()
	UAuraAbilitySystemComponent* AuraASC;

	UPROPERTY()
	AAuraCharacterBase* CharacterBase;
};
