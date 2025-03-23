// Copyright Lucas Rossi


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::FindLevelByXP(int32 XP) const
{
  for (int32 Level = 1; Level <= 50; Level++)
  {
    if (XPRequirement.AsInteger(Level) > XP)
    {
      return Level;
    }
  }

  return 1;
}

int32 ULevelInfo::GetLevelUpAttributePoints(int32 OldLevel, int32 NewLevel)
{
	int32 NewAttributePoints = 0;
	for (int32 i = NewLevel; i > OldLevel; i--)
	{
		NewAttributePoints += AttributePoints.AsInteger(i - 1);
	}

	return NewAttributePoints;
}

int32 ULevelInfo::GetLevelUpSkillPoints(int32 OldLevel, int32 NewLevel)
{
	int32 NewSkillPoints = 0;
	for (int32 i = NewLevel; i > OldLevel; i--)
	{
		NewSkillPoints += SkillPoints.AsInteger(i - 1);
	}

	return NewSkillPoints;
}

int32 ULevelInfo::GetXPRewardForTypeAndLevel(ECharacterType CharacterType, int32 Level)
{
  return XPReward.AsInteger(Level - 1) * XPRewardMultiplier[CharacterType];
}
