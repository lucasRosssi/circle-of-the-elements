// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/AttributeSetInterface.h"
#include "Interfaces/EquipperInterface.h"
#include "AuraPlayerState.generated.h"

class UEquipment;
class URune;
class USpirit;
class UAuraSaveGame;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraCharacterBase;
class ULevelInfo;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceChanged, FGameplayTag, ResourceTag, int32, Amount);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState :
  public APlayerState,
  public IAbilitySystemInterface,
  public IAttributeSetInterface,
  public IEquipperInterface
{
  GENERATED_BODY()

public:
  AAuraPlayerState();
  virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
  UAuraAbilitySystemComponent* GetAuraASC();
  UAuraAttributeSet* GetAttributeSet() const { return AttributeSet; }

  /* Attribute Set Interface */
  virtual float GetActionSpeed_Implementation() override;
  virtual void SetActionSpeed_Implementation(float InActionSpeed) override;
  virtual float GetPower_Implementation() override;
  virtual void SetTimeDilation_Implementation(float InTimeDilation) override;
  /* END Attribute Set Interface */

  /* Equipper Interface */
  virtual bool Equip_Implementation(const FGuid& ID, int32 Slot) override;
  /* END Equipper Interface*/

  int32 GetCharacterLevel() const { return Level; }
  void SetLevel(int32 InLevel);
  void AddLevel(int32 InLevel);
  int32 GetXP() const { return XP; }
  void SetXP(int32 InXP);
  void AddXP(int32 InXP);
  int32 GetAttributePoints() const { return AttributePoints; }
  void SetAttributePoints(int32 InAttributePoints);
  UFUNCTION(BlueprintCallable)
  void AddAttributePoints(int32 InAttributePoints);
  int32 GetSkillPoints() const { return SkillPoints; }
  void SetSkillPoints(int32 InSkillPoints);
  void AddSkillPoints(int32 InSkillPoints);
  void AddEquipmentToInventory(UEquipment* InEquipment);
  TArray<USpirit*> GetSpiritsInventory() const { return SpiritsInventory; }
  TArray<URune*> GetRunesInventory() const { return RunesInventory; }
  TMap<int32, FGuid> GetLoadout() const { return Loadout; }

  void AddPlayerResource(const FGameplayTag& ResourceTag, int32 Amount);
  int32 GetPlayerResourceByTag(const FGameplayTag& ResourceTag);
  bool CanAffordResourceCost(const TMap<FGameplayTag, int32>& CostMap) const;

  void InitializeState();

  FOnPlayerStatChanged OnXPChangedDelegate;
  FOnPlayerStatChanged OnLevelChangedDelegate;
  FOnPlayerStatChanged OnAttributePointsChangedDelegate;
  FOnPlayerStatChanged OnSkillPointsChangedDelegate;
  FOnPlayerStatChanged OnPerkPointsChangedDelegate;

  UPROPERTY(BlueprintAssignable)
  FOnResourceChanged OnResourceChangedDelegate;

  UPROPERTY(EditDefaultsOnly)
  TObjectPtr<ULevelInfo> LevelInfo;

protected:
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

  UPROPERTY()
  TObjectPtr<UAuraAttributeSet> AttributeSet;

  UPROPERTY(EditDefaultsOnly, Category="Level")
  bool bOverridePlayerLevel = false;

  UPROPERTY(
    EditAnywhere,
    ReplicatedUsing=OnRep_Level,
    Category="Level",
    meta=(EditCondition="bOverridePlayerLevel")
    )
  int32 Level = 1;

  UPROPERTY(
    VisibleAnywhere,
    ReplicatedUsing=OnRep_XP,
    Category="Level"
    )
  int32 XP = 0;

  UPROPERTY(
    EditAnywhere,
    ReplicatedUsing=OnRep_AttributePoints,
    Category="Level",
    meta=(EditCondition="bOverridePlayerLevel")
    )
  int32 AttributePoints = 0;

  UPROPERTY(
    EditAnywhere,
    ReplicatedUsing=OnRep_SkillPoints,
    Category="Level",
    meta=(EditCondition="bOverridePlayerLevel")
    )
  int32 SkillPoints = 0;

  UPROPERTY(
    EditAnywhere,
    BlueprintReadWrite,
    Category="Player",
    meta=(Categories="Resources", ForceInlineRow)
  )
  TMap<FGameplayTag, int32> Resources;

  UPROPERTY(
    VisibleAnywhere,
    BlueprintReadWrite,
    Category="Player"
  )
  TArray<USpirit*> SpiritsInventory;

  UPROPERTY(
    VisibleAnywhere,
    BlueprintReadWrite,
    Category="Player"
  )
  TArray<URune*> RunesInventory;

  UPROPERTY(
    VisibleAnywhere,
    BlueprintReadWrite,
    Category="Player"
  )
  TMap<int32, FGuid> Loadout;

private:
  AAuraCharacterBase* GetCharacterBase();
  UAuraSaveGame* GetSaveGame();

  USpirit* FindSpirit(const FGuid& ID);
  
  UFUNCTION()
  void OnRep_Level(int32 OldLevel);
  
  UFUNCTION()
  void OnRep_XP(int32 OldXP);
  
  UFUNCTION()
  void OnRep_AttributePoints(int32 OldAttributePoints);
  
  UFUNCTION()
  void OnRep_SkillPoints(int32 OldSkillPoints);

  UPROPERTY()
  UAuraAbilitySystemComponent* AuraASC;

  UPROPERTY()
  AAuraCharacterBase* CharacterBase;

  UPROPERTY()
  UAuraSaveGame* SaveGame;

  bool bInitialized = false;
};
