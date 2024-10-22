// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Enums/CharacterName.h"
#include "AuraGameInstance.generated.h"

struct FHeroData;
class UHeroInfo;
class AAuraHero;
class UUpgradeInfo;
class URewardsInfo;
class URegionInfo;
class UStatusEffectInfo;
class UAbilityInfo;
class UCharacterInfo;
enum class ECharacterName : uint8;

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SaveHeroData();

  ECharacterName GetCurrentCharacterName() const { return CurrentHeroName; }
  FHeroData GetCurrentHeroData() const;

  UAbilityInfo* GetAbilityInfo() const { return AbilityInfo; }
  UCharacterInfo* GetCharacterInfo() const { return CharacterClassInfo; }
  UHeroInfo* GetHeroInfo() const { return HeroInfo; }
  URegionInfo* GetRegionInfo() const { return RegionInfo; }
  URewardsInfo* GetRewardsInfo() const { return RewardsInfo; }
  UStatusEffectInfo* GetStatusEffectInfo() const { return StatusEffectInfo; }
  UUpgradeInfo* GetUpgradeInfo() const { return UpgradeInfo; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player|Hero")
	ECharacterName CurrentHeroName = ECharacterName::Aura;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UAbilityInfo> AbilityInfo;
  
  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UCharacterInfo> CharacterClassInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UHeroInfo> HeroInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<URegionInfo> RegionInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<URewardsInfo> RewardsInfo;
  
  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UStatusEffectInfo> StatusEffectInfo;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Info")
  TObjectPtr<UUpgradeInfo> UpgradeInfo;
  
private:
};
