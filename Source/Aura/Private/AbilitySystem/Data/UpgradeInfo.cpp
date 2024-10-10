// Copyright Lucas Rossi


#include "AbilitySystem/Data/UpgradeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraUpgradeInfo UUpgradeInfo::FindUpgradeInfoByTag(const FGameplayTag& UpgradeTag, bool bLogNotFound) const
{
  for (const auto Data : Upgrades)
  {
    for (auto Element : Data.Value.Elements)
    {
      FAuraUpgradeInfo* Info = Element.Value.UpgradeList.Find(UpgradeTag);
      if (Info != nullptr)
      {
        return *Info;
      }
    }
  }

  if (bLogNotFound)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Can't find Info for UpgradeTag [%s] on UpgradeInfo [%s]"), 
      *UpgradeTag.ToString(), *GetNameSafe(this)
    );
  }

  return FAuraUpgradeInfo();
}

FUpgradesMapStruct UUpgradeInfo::FindCharacterUpgrades(ECharacterName CharacterName, bool bLogNotFound) const
{
  const FUpgradesMapStruct* CharUpgrades = Upgrades.Find(CharacterName);
  if (CharUpgrades != nullptr)
  {
    return *CharUpgrades;
  }

  if (bLogNotFound)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Can't find Upgrades Info for Character [%hhd] on UpgradeInfo [%s]"), 
      CharacterName, *GetNameSafe(this)
    );
  }

  return FUpgradesMapStruct();
}

TMap<FGameplayTag, FAuraUpgradeInfo> UUpgradeInfo::FindCharacterUpgradesOfElement(
  ECharacterName CharacterName,
  const FGameplayTag& ElementTag,
  bool bLogNotFound
) const
{
  const FUpgradesMapStruct& CharacterUpgrades = FindCharacterUpgrades(CharacterName, bLogNotFound);
  const FUpgradeListMapStruct* List = CharacterUpgrades.Elements.Find(ElementTag);

  if (List != nullptr)
  {
    return List->UpgradeList;
  }

  if (bLogNotFound)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Can't find Upgrades of Element [%s] for Character [%hhd] on UpgradeInfo [%s]"),
      *ElementTag.ToString(),
      CharacterName,
      *GetNameSafe(this)
    );
  }

  return TMap<FGameplayTag, FAuraUpgradeInfo>();
}

FAuraUpgradeInfo UUpgradeInfo::FindUpgradeInfoWithParams(const FUpgradeInfoParams& Params, bool bLogNotFound) const
{
  TMap<FGameplayTag, FAuraUpgradeInfo> AbilityInfos = FindCharacterUpgradesOfElement(
    Params.CharacterName,
    Params.ElementTag,
    bLogNotFound
    );

  const FAuraUpgradeInfo* Info = AbilityInfos.Find(Params.UpgradeTag);
  if (Info != nullptr)
  {
    return *Info;
  }

  if (bLogNotFound)
  {
    UE_LOG(
      LogAura,
      Error,
      TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]"), 
      *Params.UpgradeTag.ToString(),
      *GetNameSafe(this)
    );
  }

  return FAuraUpgradeInfo();
}
