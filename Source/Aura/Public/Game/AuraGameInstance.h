// Copyright Lucas Rossi

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Enums/CharacterName.h"
#include "Enums/Region.h"
#include "AuraGameInstance.generated.h"

class UEnemiesInfo;
class ULevelInfo;
struct FSaveInfo;
class UAuraSaveGame;
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
  void SaveGameData(const FSaveInfo& SaveData);
  void SaveCurrentGame();
  UAuraSaveGame* LoadGameData(int32 SlotIndex);
  void DeleteGameData(int32 SlotIndex);
  void LoadAndPlay(int32 SlotIndex);

  ECharacterName GetCurrentCharacterName() const { return CurrentHeroName; }
  FHeroData GetCurrentHeroData() const;
  ERegion GetCurrentRegion() const { return CurrentRegion; }
  void SetCurrentRegion(ERegion Region) { CurrentRegion = Region; }
  FName GetCurrentLocation() const { return CurrentLocation; }
  void SetCurrentLocation(FName Location) { CurrentLocation = Location; }

  UAbilityInfo* GetAbilityInfo() const { return AbilityInfo; }
  UCharacterInfo* GetCharacterInfo() const { return CharacterClassInfo; }
  UHeroInfo* GetHeroInfo() const { return HeroInfo; }
  URegionInfo* GetRegionInfo() const { return RegionInfo; }
  UEnemiesInfo* GetEnemiesInfo() const { return EnemiesInfo; }
  URewardsInfo* GetRewardsInfo() const { return RewardsInfo; }
  UStatusEffectInfo* GetStatusEffectInfo() const { return StatusEffectInfo; }
  UUpgradeInfo* GetUpgradeInfo() const { return UpgradeInfo; }
  ULevelInfo* GetLevelInfo() const { return LevelInfo; };

  UAuraSaveGame* GetCurrentSaveGameObject() const { return SaveGame; }
  FString GetPlayerName() const { return PlayerName; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player|Hero")
	ECharacterName CurrentHeroName = ECharacterName::Aura;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Location")
  ERegion CurrentRegion = ERegion::Undefined;
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Location")
  FName CurrentLocation = NAME_None;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UAbilityInfo> AbilityInfo;
  
  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UCharacterInfo> CharacterClassInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UHeroInfo> HeroInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<URegionInfo> RegionInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UEnemiesInfo> EnemiesInfo;

  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<URewardsInfo> RewardsInfo;
  
  UPROPERTY(EditDefaultsOnly, Category="Info")
  TObjectPtr<UStatusEffectInfo> StatusEffectInfo;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Info")
  TObjectPtr<UUpgradeInfo> UpgradeInfo;
  
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Info")
  TObjectPtr<ULevelInfo> LevelInfo;
  
private:
  FString GetSlotName(int32 SlotIndex) const;
  
  UPROPERTY()
  TObjectPtr<UAuraSaveGame> SaveGame;
  
  FString PlayerName;
};
