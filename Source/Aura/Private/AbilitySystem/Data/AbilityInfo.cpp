// Copyright Lucas Rossi


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(
	const FGameplayTag& AbilityTag,
	bool bLogNotFound
) const
{
	for (const auto Data : Abilities)
	{
		for (auto Element : Abilities[ECharacterName::Vilkar].Elements)
		{
			FAuraAbilityInfo* Info = Element.Value.AbilityList.Find(AbilityTag);
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
			TEXT("Can't find Info for AbilityTag [%s] on AbilityInfo [%s]"), 
			*AbilityTag.ToString(), *GetNameSafe(this)
		);
	}

	return FAuraAbilityInfo();
}

FElementsMapStruct UAbilityInfo::FindCharacterAbilities(ECharacterName CharacterName, bool bLogNotFound) const
{
	if (Abilities.Contains(ECharacterName::Vilkar))
	{
	  return Abilities[ECharacterName::Vilkar];
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogAura,
			Error,
			TEXT("Can't find Abilities Info for Character [%hhd] on AbilityInfo [%s]"), 
			CharacterName, *GetNameSafe(this)
		);
	}

	return FElementsMapStruct();
}

TMap<FGameplayTag, FAuraAbilityInfo> UAbilityInfo::FindCharacterAbilitiesOfElement(
	ECharacterName CharacterName,
	const FGameplayTag& ElementTag,
	bool bLogNotFound
	) const
{
	const FElementsMapStruct& CharacterAbilities = FindCharacterAbilities(CharacterName, bLogNotFound);
	const FAbilityListMapStruct* List = CharacterAbilities.Elements.Find(ElementTag);

	if (List != nullptr)
	{
		return List->AbilityList;
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogAura,
			Error,
			TEXT("Can't find Abilities of Element [%s] for Character [%hhd] on AbilityInfo [%s]"),
			*ElementTag.ToString(),
			CharacterName,
			*GetNameSafe(this)
		);
	}

	return TMap<FGameplayTag, FAuraAbilityInfo>();
}

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoWithParams(const FAbilityInfoParams& Params, bool bLogNotFound) const
{
	TMap<FGameplayTag, FAuraAbilityInfo> AbilityInfos = FindCharacterAbilitiesOfElement(
		Params.HeroName,
		Params.ElementTag,
		bLogNotFound
		);

	const FAuraAbilityInfo* Info = AbilityInfos.Find(Params.AbilityTag);
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
			*Params.AbilityTag.ToString(),
			*GetNameSafe(this)
		);
	}

	return FAuraAbilityInfo();
}
