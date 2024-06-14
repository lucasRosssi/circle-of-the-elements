// Copyright Lucas Rossi


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(
	const FGameplayTag& AbilityTag,
	bool bLogNotFound
) const
{
	for (const auto Info : CharacterAbilities)
	{
		for (auto AbilityInfo : Info.Value.AbilityInformation)
		{
			if (AbilityInfo.AbilityTag == AbilityTag)
			{
				return AbilityInfo;
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

TArray<FAuraAbilityInfo> UAbilityInfo::FindAbilitiesFromCharacter(
	ECharacterName CharacterName,
	bool bLogNotFound
) const
{
	if (CharacterAbilities.Contains(CharacterName))
	{
		return CharacterAbilities.Find(CharacterName)->AbilityInformation;
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

	return TArray<FAuraAbilityInfo>();
}
