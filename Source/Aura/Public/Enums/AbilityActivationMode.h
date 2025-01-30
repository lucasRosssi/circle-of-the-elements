#pragma once

UENUM(BlueprintType)
enum class EAbilityActivationMode : uint8
{
	// Ability just triggers and is not affected by external factors.
	Default UMETA(DisplayName = "Default"),
	// Ability needs to be constantly channeled. Stopping or being affected by incapacitations cancels it.
	Channeling UMETA(DisplayName = "Channeling"),
	// Ability toggles activation and constantly spend mana, but the character can do other things. Is canceled by incapacitations.
	Concentration UMETA(DisplayName = "Concentration"),
  // Ability can be charged up to amplify its effects. Ability will be canceled in the focus phase if character is affected by incapacitations.
  Focus UMETA(DisplayName = "Focus"),

	MAX UMETA(Hidden)
};
