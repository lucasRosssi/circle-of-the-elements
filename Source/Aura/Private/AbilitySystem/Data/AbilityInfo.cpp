// Copyright Lucas Rossi


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

void UAbilityInfo::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
  Super::PostEditChangeChainProperty(PropertyChangedEvent);

  const FName MemberPropertyName = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetFName();

  if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UAbilityInfo, Abilities))
  {
    for (auto& [ElementTag, ElementInfo] : Abilities)
    {
      for (auto& [AbilityTag, AbilityInfo] : ElementInfo.AbilityList)
      {
        AbilityInfo.ElementTag = ElementTag;
        AbilityInfo.AbilityTag = AbilityTag;
        AbilityInfo.BackgroundMaterial = ElementInfo.BackgroundMaterial;
      }
    }
  }
}

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(
	const FGameplayTag& AbilityTag
) const
{
	for (auto Element : Abilities)
	{
		FAuraAbilityInfo* Info = Element.Value.AbilityList.Find(AbilityTag);
		if (Info != nullptr)
		{
		  Info->ElementTag = Element.Key;
			return *Info;
		}
	}

	UE_LOG(
		LogAura,
		Error,
		TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]"), 
		*AbilityTag.ToString(), *GetNameSafe(this)
	);
	

	return FAuraAbilityInfo();
}

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityInfo::FindElementAbilities(const FGameplayTag& ElementTag) const
{
	return FindElementInfo(ElementTag).AbilityList;
}

FAbilityElementInfo UAbilityInfo::FindElementInfo(const FGameplayTag& ElementTag) const
{
  const FAbilityElementInfo* ElementInfo = Abilities.Find(ElementTag);

  if (ElementInfo)
  {
    return *ElementInfo;
  }
  
  UE_LOG(
    LogAura,
    Error,
    TEXT("Can't find Element [%s] on AbilityInfo [%s]"),
    *ElementTag.ToString(),
    *GetNameSafe(this)
  );
  
  return FAbilityElementInfo();
}

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoWithParams(const FAbilityInfoParams& Params) const
{
	TMap<FGameplayTag, FAuraAbilityInfo> AbilityInfos = FindElementAbilities(Params.ElementTag);

	const FAuraAbilityInfo* Info = AbilityInfos.Find(Params.AbilityTag);
	if (Info != nullptr)
	{
		return *Info;
	}

	UE_LOG(
		LogAura,
		Error,
		TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]"), 
		*Params.AbilityTag.ToString(),
		*GetNameSafe(this)
	);
  
	return FAuraAbilityInfo();
}
