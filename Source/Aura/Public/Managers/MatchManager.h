// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Managers/AuraSystemComponent.h"
#include "MatchManager.generated.h"

class UEquipment;
class AAuraPlayerState;

USTRUCT(BlueprintType, Blueprintable)
struct FMatchScores
{
  GENERATED_BODY()

  UPROPERTY(VisibleAnywhere)
  int32 Kills = 0;
  UPROPERTY(VisibleAnywhere)
  float DamageDone = 0.f;
  UPROPERTY(VisibleAnywhere)
  float DamageTaken = 0.f;
  UPROPERTY(VisibleAnywhere)
  float DamagePrevented = 0.f;
  UPROPERTY(VisibleAnywhere)
  int32 SuccessfulDodges = 0;
  UPROPERTY(VisibleAnywhere)
  float ManaSpent = 0.f;
  UPROPERTY(VisibleAnywhere)
  int32 StatusEffectsApplied = 0;
  UPROPERTY(VisibleAnywhere)
  FGameplayTag MostAppliedStatusEffect;
  UPROPERTY(VisibleAnywhere)
  float XP = 0.f;
};

/**
 * 
 */
UCLASS()
class AURA_API UMatchManager : public UAuraSystemComponent
{
	GENERATED_BODY()

public:
  UFUNCTION(BlueprintPure)
  FMatchScores GetMatchScores() const { return Scores; }
  
  void RegisterKills(int32 KillAmount);
  void RegisterDamageDone(float Damage);
  void RegisterDamageTaken(float Damage);
  void RegisterDamagePrevented(float Damage);
  void RegisterSuccessfulDodges(int32 Amount);
  void RegisterManaSpent(float Mana);
  void RegisterStatusEffectsApplied(int32 Amount);
  void RegisterXP(int32 XP);
  void RegisterLoot(UEquipment* Equipment);

  UFUNCTION(BlueprintCallable)
  void EndMatch(bool bVictory);
  
protected:
  AAuraPlayerState* GetAuraPlayerState();
  
  UPROPERTY(VisibleAnywhere)
  FMatchScores Scores = FMatchScores();
  
private:
  UPROPERTY()
  TWeakObjectPtr<AAuraPlayerState> AuraPlayerState = nullptr;

  UPROPERTY()
  TArray<UEquipment*> Loot;
};
