// Copyright Lucas Rossi


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::FindLevelByXP(int32 XP) const
{
	const FLevelUpInfo* FoundInfo = LevelInformation.FindByPredicate(
		[this, XP](FLevelUpInfo Item)
	{
		return XP < Item.XPRequirement;
	});
	
	return FoundInfo->Level - 1;
}

int32 ULevelInfo::GetLevelUpAttributePoints(int32 OldLevel, int32 NewLevel)
{
	int32 AttributePoints = 0;
	for (int32 i = NewLevel; i <= OldLevel; i--)
	{
		AttributePoints += LevelInformation[i - 1].AttributePoints;
	}

	return AttributePoints;
}
