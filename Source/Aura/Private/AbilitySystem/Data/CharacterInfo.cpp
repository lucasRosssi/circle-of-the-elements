// Copyright Lucas Rossi


#include "AbilitySystem/Data/CharacterInfo.h"

FCharacterClassDefaultInfo UCharacterInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
